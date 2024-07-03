// Fill out your copyright notice in the Description page of Project Settings.


#include "EventActor.h"
#include "Components/BoxComponent.h"
#include "DialogueInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UIGameMode.h"
#include "Components/WidgetComponent.h"
#include "MarkerWidget.h"
#include "MyCharacter.h"
#include "Saveload/SavedEventActor.h"
#include "Containers/Queue.h"
//#include "SaveLoad/SaveSystem.h"
#include "../UI/PauseUI.h"
#include "ExecutionInterface.h"

// Sets default values
AEventActor::AEventActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Collider"));
	SetRootComponent(Collider);
	DialogueDatas = MakeShareable(new TQueue<TSharedPtr<DialogueData>>());
	ExecutionEvents = TArray<AActor*>();
	EventIndexes = TMap<FString, int>();
	EventMap = TMap<FString, EventFunction>();
	EventSequence = TArray<FEventStruct>();
	Events = TArray<TArray<EventFunction>>();
	IsCompletedEvent = false;
	IsOngoingEvent = false;
	SetEventIndexes();
	SetEventMaps();
	//Events = TArray<TArray<EventDelegate>>();
	//Events = TArray<EventFunction>();
	//Fp = &AEventActor::TestFunction;
	//Events.Add(&AEventActor::TestFunction);
	//Events.Add(&AEventActor::TestFunction2);
	DialogueTimer = 0.f;
	static ConstructorHelpers::FClassFinder<UMarkerWidget>WidgetAsset(TEXT("/Game/UI/BP/WBP_MarkerMeter.WBP_MarkerMeter_C"));
	if (WidgetAsset.Succeeded())
	{
		WidgetClass = WidgetAsset.Class;
	}
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Marker"));
	Widget->SetupAttachment(RootComponent);
	EventCount = 0;
	CurrentEventCount = 0;
	DialogueCount = 0;
	ContDialogueCount = TArray<int>();
	IsSavePoint = false;
	IsFirstTrigger = false;
	IsDisplayingMarker = true;
	//Widget->GetWidget()
}

// Called when the game starts or when spawned
void AEventActor::BeginPlay()
{
	Super::BeginPlay();
	true == IsFirstTrigger ? IsOnActive = true : IsOnActive = false;
	SetActorToSavableList_Implementation();
	LoadDialogues();
	SetEventFunctions();
	CreateMarkerWidget();
	LoadData();
	if ((true == DisplayMarkerOnBeginPlay || true == IsOnActive) && false == IsCompletedEvent)
	{
		ActivateMarker();
	}
	else
	{
		SetMarkerVisibility(false);
	}
}

// Called every frame
void AEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (true == IsMarkerActived)
	{
		UpdateDistance();
	}
	if (false == IsOnActive)
	{
		return;
	}
	if (false == IsOngoingEvent)
	{
		return;
	}
	if (true == IsCurrentEventsCompleted())
	{
		UE_LOG(LogTemp, Log, TEXT("Execute next events"));
		ExecuteEvents();
	}
}

void AEventActor::CompleteTrigger()
{
	IsOngoingEvent = false;
	IsOnActive = false;
	IsCompletedEvent = true;
	Events.Empty();
	DisableTrigger();
}

bool AEventActor::LoadData()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	TSharedPtr<FJsonObject> json = inst->FindJsonByTag(this->Tags.IsEmpty() ? FName("") : this->Tags[0]);
	if (nullptr == json)
	{
		UE_LOG(LogTemp, Log, TEXT("Can not find saved data for %s"), *GetName());
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Found saved data for %s"), *GetName());
	FSavedEventActor data;
	data.SetDeserializedData(json, this);
	return true;
}

void AEventActor::SetActorToSavableList_Implementation()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	if (nullptr == inst)
	{
		UE_LOG(LogTemp, Log, TEXT("Failed to get game instance"));
		return;
	}
	inst->SetActorToList(this);
}

FString AEventActor::JsonSerialize_Implementation()
{
	FSavedEventActor data = FSavedEventActor(this);
	FString str = data.JsonSerialize();
	//UE_LOG(LogTemp, Log, TEXT("Serialized %s : \n %s"), *GetActorLabel(), *str);
	return str;
}

bool AEventActor::JsonDeserialize_Implementation()
{
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	TSharedPtr<FJsonObject> json = inst->FindJsonByTag(this->Tags.IsEmpty() ? FName("") : this->Tags[0]);
	if (nullptr == json)
	{
		UE_LOG(LogTemp, Log, TEXT("Can not find saved data for %s"), *GetName());
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("Found saved data for %s"), *GetName());
	FSavedEventActor data;
	data.SetDeserializedData(json, this);
	if (true == IsDisplayingMarker) SetMarkerVisibility(IsMarkerActived);
	return true;
}

bool AEventActor::IsEventQueueEmpty()
{
	return Events.IsEmpty();
}

void AEventActor::ExecuteEvents()
{
	if (true == Events.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Event function array is empty"));
		return;
	}
	TArray<EventFunction> events = Events[0];
	Events.RemoveAt(0);
	CurrentEventCount = 0;
	EventCount = events.Num();
	for (auto& _event : events)
	{
		(this->*_event)();
	}
}

void AEventActor::UpdateEventCount()
{
	EventCount = Events[0].Num();

}

void AEventActor::UpdateEventState()
{
	AddEventCount();
	IsCompletedEvent = IsEventQueueEmpty() && IsCurrentEventsCompleted();
	UE_LOG(LogTemp, Log, TEXT("Event State updated, current event count is %d/%d"), CurrentEventCount, EventCount);
	UE_LOG(LogTemp, Log, TEXT("Event State updated, left eventgroup count is %d"), IsEventQueueEmpty() == true ? -1 : Events.Num());
	
	if (false == IsCompletedEvent)
	{
		return;
	}
	IsOngoingEvent = false;
	DisableTrigger();
	if (DestinationGroup.IsEmpty()) 
	{
		if (nullptr != Destination) Destination->EnableTrigger();
		else UE_LOG(LogTemp, Log, TEXT("Trigger error : Next trigger is not set"));
	}
	else
	{
		for (auto& trigger : DestinationGroup)
		{
			if (nullptr != trigger) trigger->EnableTrigger();
			else UE_LOG(LogTemp, Log, TEXT("Trigger error : Next trigger is not set"));
		}
	}
}

void AEventActor::AddEventCount()
{
	CurrentEventCount++;

}

inline bool AEventActor::IsCurrentEventsCompleted()
{
	return CurrentEventCount >= EventCount;
}

void AEventActor::SetEventMaps()
{
	int i = 0;
	EventMap.Add(EventName[i++], &AEventActor::ExecuteDialogue);
	EventMap.Add(EventName[i++], &AEventActor::ExecuteExecutionEvent);
	EventMap.Add(EventName[i++], &AEventActor::OpenLevel);
	//EventMap.Add(EventName[i++], &AEventActor::SaveGame);
	EventMap.Add(EventName[i++], &AEventActor::SetDestinationMarker);
	EventMap.Add(EventName[i++], &AEventActor::ExecuteTimer);
	EventMap.Add(EventName[i++], &AEventActor::ExecuteNightVisionEvent);
	EventMap.Add(EventName[i++], &AEventActor::ExecuteLimitTimer);
}

void AEventActor::SetEventFunctions()
{
	int res = 0;
	int dlgInitIndex = 0;	
	if (true == IsDestination || true == DisplayMarkerOnBeginPlay)
	{
		if (true == Events.IsEmpty())
		{
			Events.Add(TArray<EventFunction>());
		}
		if (true == IsDisplayingMarker) Events[0].Add(&AEventActor::RemoveMarker);
	}
	if (false == EventSequence.IsEmpty())
	{
		int lastSeq = GetLastEventSequence();
		EventFunction temp;
		FString str;
		while (Events.Num() <= lastSeq)
		{
			Events.Add(TArray<EventFunction>());
		}

		for (auto& ev : EventSequence)
		{
			res = EventName.Find(ev.EventName);
			if (INDEX_NONE == res)
			{
				//UE_LOG(LogTemp, Log, TEXT("Event error : Wrong event name in %s, EventName : %s, EventSequence : %d"), *GetActorLabel(), *ev.EventName, ev.Sequence);
				continue;
			}
			if (true == IsDialogueEvent(ev.EventName))
			{
				int dlgCount = 0;
				if (false == ContDialogueCount.IsValidIndex(dlgInitIndex))
				{
					//UE_LOG(LogTemp, Log, TEXT("Event error : Invalid dialogue row name in %s"), *GetActorLabel());
					continue;
				}
				dlgCount = ContDialogueCount[dlgInitIndex];
				for (int i = 0; i <= dlgCount; i++)
				{
					if (Events.Num() < ev.Sequence + i)
					{
						//UE_LOG(LogTemp, Log, TEXT("Event error in %s"), *GetActorLabel());
					}
					else 
					{
						Events[ev.Sequence + i].Insert(EventMap[ev.EventName], 0);
					}
				}
				IsSeqBasedOnDialogue = true;
				dlgInitIndex++;
				continue;
			}
			else
			{
				if (Events.Num() < ev.Sequence)
				{
					//UE_LOG(LogTemp, Log, TEXT("Event error in %s"), *GetActorLabel());
				}
				else
				{
					str = ev.EventName;
					temp = EventMap[ev.EventName];
					Events[ev.Sequence].Add(EventMap[ev.EventName]);
				}
			}
			//UE_LOG(LogTemp, Log, TEXT("%s event is added in %s"), *ev.EventName, *GetActorLabel());
		}
	}
	if (IsSavePoint)
	{
		Events.Add(TArray<EventFunction>());
		Events.Last().Add(&AEventActor::SaveGame);
	}
}

inline bool AEventActor::IsValidEventIndex(const int& Sequence)
{
	return Events.IsValidIndex(Sequence);
}

bool AEventActor::IsDialogueEvent(const FString& Name)
{
	return EventName[0] == Name;
}

int AEventActor::GetLastEventSequence()
{
	int seq = GetLastDialogueSequence() + DialogueCount - 1;
	if (seq < 0) 
	{
		return EventSequence.Last().Sequence;
	}
	return EventSequence.Last().Sequence < seq ? seq : EventSequence.Last().Sequence;
}

int AEventActor::GetLastDialogueSequence()
{
	int seq = 0;
	int size = EventSequence.Num();
	for (int i = size - 1; i >= 0; i--)
	{
		if (EventSequence[i].EventName == EventName[0])
		{
			seq = EventSequence[i].Sequence;
			return seq;
		}
	}
	return -1;
}

void AEventActor::EnableTrigger()
{
	//SetActorTickEnabled(true);
	//Collider->SetGenerateOverlapEvents(true);
	IsOnActive = true;
}

void AEventActor::DisableTrigger()
{
	UE_LOG(LogTemp, Log, TEXT("Disableactor called"));
	SetActorTickEnabled(false);
	//Collider->SetGenerateOverlapEvents(false);
	SetMarkerVisibility(false);
	IsOnActive = false;
	IsMarkerActived = false;
}

void AEventActor::StopTimers()
{
	if (true == GetWorldTimerManager().IsTimerActive(DialogueTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DialogueTimerHandle);
	}
	if (true == GetWorldTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	//GetWorldTimerManager().ClearAllTimersForObject(GetWorld());
}

void AEventActor::SetEventIndexes()
{
	int EventTypes = EventName.Num();
	for (int i = 0; i < EventTypes; i++)
	{
		EventIndexes.Add(EventName[i], i);
	}
}

void AEventActor::CreateMarkerWidget()
{
	if (false == IsDestination) return;
	if (nullptr != WidgetClass)
	{
		UMarkerWidget* widget = Cast<UMarkerWidget>(CreateWidget(GetWorld(), WidgetClass));
		if (Widget != nullptr)
		{
			Widget->SetWidget(widget);
			Widget->SetWidgetSpace(EWidgetSpace::Screen);
			if (true == IsDisplayingMarker) SetMarkerVisibility(false);
		}
	}
}

void AEventActor::OnInteract_Implementation(AMyCharacter* Target)
{
	if (false == IsManualEvent || true == IsOngoingEvent || true == IsCompletedEvent || false == IsOnActive)
	{
		UE_LOG(LogTemp, Log, TEXT("Trigger exception return"));
		return;
	}
	IsOngoingEvent = true;
	//�ӽ�
	if (false == GroupedTriggers.IsEmpty())
	{
		for (auto& trigger : GroupedTriggers)
		{
			trigger->CompleteTrigger();
		}
	}
	PlayInteractionAnimation();
	ExecuteEvents();
}

void AEventActor::OnInteractRangeEntered_Implementation(AMyCharacter* Target)
{
	if (true == IsManualEvent || true == IsOngoingEvent || true == IsCompletedEvent || false == IsOnActive)
	{
		UE_LOG(LogTemp, Log, TEXT("Trigger exception return"));
		return;
	}
	IsOngoingEvent = true;
	if (false == GroupedTriggers.IsEmpty())
	{
		for (auto& trigger : GroupedTriggers)
		{
			trigger->CompleteTrigger();
		}
	}
	ExecuteEvents();
}

void AEventActor::OnInteractRangeExited_Implementation(AMyCharacter* Target)
{

}

bool AEventActor::GetOverlapEventState()
{
	return Collider->GetGenerateOverlapEvents();
}

//�ӽ�
void AEventActor::PlayInteractionAnimation()
{
	K2_PlayInteractionAnimation();
}

void AEventActor::LoadDialogues()
{
	TSharedPtr<DialogueData> data = nullptr;
	FDialogueData* dialogue = nullptr;
	int contCount = 0;
	if (true == InitialRowName.IsEmpty()) return;
	for (auto& curRowName : InitialRowName)
	{
		while (true)
		{
			UE_LOG(LogTemp, Log, TEXT("Dialogue RowName = %s"), *curRowName);
			dialogue = GetDialogueData(curRowName);
			if (nullptr == dialogue)
			{
				//UE_LOG(LogTemp, Log, TEXT("Event error : Invalid dialogue row name in %s, row name : %s"), *GetActorLabel(), *curRowName);
				break;
			}
			data = MakeShareable<DialogueData>(new DialogueData(dialogue));
			if (nullptr == data)
			{
				UE_LOG(LogTemp, Log, TEXT("Invalid dialogue data, current row name = %s"), *curRowName);
				break;
			}
			EnqueueDialogueData(data);
			if (false == IsHavingNextDialogue(dialogue))
			{
				UE_LOG(LogTemp, Log, TEXT("End of dialogue queue, last row name = %s"), *curRowName);
				ContDialogueCount.Add(contCount);
				break;
			}
			curRowName = dialogue->con_After;
			contCount++;
		}
	}
}

inline FDialogueData* AEventActor::GetDialogueData(FString RowName)
{
	return FString("") == RowName ? nullptr : GetWorld()->GetGameInstance<UDialogueInstance>()->GetDialogueData(RowName);
}

inline void AEventActor::EnqueueDialogueData(const TSharedPtr<DialogueData> Data)
{
	DialogueDatas.Get()->Enqueue(Data);
	DialogueCount++;
}

inline bool AEventActor::IsHavingNextDialogue(const FDialogueData* Data)
{
	return TEXT("") != Data->con_After;
}

void AEventActor::ExecuteDialogue()
{
	TSharedPtr<DialogueData> data = nullptr;
	if (true == IsQueueEmpty())
	{
		HideDialogueWidget();
		return;
	}
	DialogueDatas.Get()->Dequeue(data);
	if (nullptr == data)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid dequeued dialogue data"));
		return;
	}
	DialogueTimer = data.Get()->Audio->Duration + (0.f == data.Get()->Delay ? 1.f : data.Get()->Delay);
	UpdateDialogueWidget(data);

	EventFunction DialogueTimerCallback = DialogueDatas->IsEmpty() ? &AEventActor::HideDialogueWidget : &AEventActor::UpdateEventState;
	GetWorldTimerManager().SetTimer(DialogueTimerHandle, this, DialogueTimerCallback, DialogueTimer, false);
}

void AEventActor::UpdateDialogueWidget(const TSharedPtr<DialogueData> Data)
{
	if (Data.IsValid() == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Dialogue widget updated"));
		Cast<AUIGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->UpdateDialogueWidget(Data->Speaker, Data.Get()->DialogueText);
		UGameplayStatics::PlaySound2D(this, Data.Get()->Audio);
	}
}

inline bool AEventActor::IsQueueEmpty()
{
	return DialogueDatas.Get()->IsEmpty();
}

inline void AEventActor::HideDialogueWidget()
{
	UpdateEventState();
	Cast<AUIGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->HideDialogueWidget();
}

void AEventActor::ExecuteExecutionEvent()
{
	if (true == ExecutionEvents.IsEmpty())
	{
		//UE_LOG(LogTemp, Log, TEXT("No execution event in %s"), *GetActorLabel());
		return;
	}
	for (auto& actor : ExecutionEvents)
	{
		if (actor->GetClass()->ImplementsInterface(UExecutionInterface::StaticClass())) IExecutionInterface::Execute_ExecuteEvent(actor);
	}
	UpdateEventState();
}

void AEventActor::ActivateMarker()
{
	SetMarkerVisibility(true);
	UpdateDistance();
	IsMarkerActived = true;
}

void AEventActor::SetMarkerVisibility(bool Visib)
{
	Widget->SetVisibility(Visib);
}

inline void AEventActor::OpenLevel()
{
	FLatentActionInfo LActionInfo;
	UGameplayStatics::LoadStreamLevel(this, FName(LevelRef), true, true, LActionInfo);
	UGameplayStatics::OpenLevel(this, FName(LevelRef));
	StopTimers();
	UpdateEventState();
}

void AEventActor::SaveGame()
{
	UpdateEventState();
	if (nullptr != SaveLocation) Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))->SetSavedLocation(SaveLocation->GetActorLocation());
	else UE_LOG(LogTemp, Log, TEXT("Trigger error : save location is not set"));
	UDialogueInstance* inst = Cast<UDialogueInstance>(UGameplayStatics::GetGameInstance(this));
	inst->SaveActorToJson(SaveLocation->GetActorLocation());
}

void AEventActor::SetDestinationMarker()
{
	if (nullptr == Destination)
	{
		UE_LOG(LogTemp, Log, TEXT("Please set destination in %s"), *GetName());
		return;
	}
	if (true == IsDisplayingMarker) Destination->ActivateMarker();
	UpdateEventState();
}

void AEventActor::RemoveMarker()
{
	SetMarkerVisibility(false);
	UpdateEventState();
}

void AEventActor::UpdateDistance()
{
	if (nullptr == UGameplayStatics::GetPlayerCharacter(this, 0) || nullptr == Widget->GetWidget())
	{
		return;
	}
	Cast<UMarkerWidget>(Widget->GetWidget())->UpdateDistance(FVector::Distance(this->GetActorLocation(),
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation()));
}

void AEventActor::ExecuteTimer()
{
	AUIGameMode* gm = Cast<AUIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gm->SetTimerWidget(InitialTime, FirstLimit, FinalLimit, IsUsingWarning);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEventActor::TimerWaiter, InitialTime, false);
}

void AEventActor::TimerWaiter()
{
	UE_LOG(LogTemp, Log, TEXT("Timer over"));
	UpdateEventState();
}

void AEventActor::ExecuteNightVisionEvent()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (nullptr == Controller)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid character controller"));
		return;
	}
	AMyCharacter* Character = Cast<AMyCharacter>(Controller->GetPawn());
	if (nullptr == Character)
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid character object"));
		return;
	}
	if (true == Character->IsVisionStateModified(NightVisionMode))
	{
		Character->UpdateVisionState(NightVisionMode);
		Character->Vision(NightVisionMode ? true : false);
	}
	else
	{
		IsOngoingEvent = false;
	}
	UpdateEventState();
}

void AEventActor::ExecuteLimitTimer()
{
	AUIGameMode* gm = Cast<AUIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gm->SetTimerWidget(InitialTime, FirstLimit, FinalLimit, IsUsingWarning);
	UpdateEventState();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEventActor::TimeOver, InitialTime, false);
}

void AEventActor::TimeOver()
{
	AUIGameMode* gm = Cast<AUIGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	Events.Empty();
	StopTimers();
	gm->GetPauseHUDWidget()->DeadPauseOn();
}

AEventActor::DialogueData::DialogueData(const FDialogueData* Data)
{
	Audio = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), nullptr, *Data->sound_Ref));
	Speaker = Data->Speaker;
	DialogueText = Data->text;
	Delay = Data->time_delete;
	UE_LOG(LogTemp, Log, TEXT("Dialogue loaded, audio name = %s, text = %s, delay = %f"), *Audio->GetName(), *DialogueText, Delay);
}

