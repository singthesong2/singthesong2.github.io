// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SaveLoad/SavableInterface.h"
#include "InteractInterface.h"
#include "Sound/SoundCue.h"
#include <functional>
#include "DialogueTable.h"

#include "EventActor.generated.h"
//DECLARE_DELEGATE(EventDelegate);

USTRUCT(BlueprintType)
struct FEventStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Sequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventName;
};

UCLASS()
class SPECTER_API AEventActor : public AActor, public IInteractInterface, public ISavableInterface
{
	GENERATED_BODY()
protected:
	struct DialogueData
	{
		USoundCue* Audio;
		SpeakerType Speaker;
		FString DialogueText;
		float Delay;
		DialogueData(const struct FDialogueData* Data);
	};
	const TArray<FString> EventName =
	{ "Dialogue", "Execution", "LevelTransition",
		"Marker", "Timer", "NightVision", "TimeLimit" };
public:
	// Sets default values for this actor's properties
	AEventActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* Collider;
	UPROPERTY(VisibleAnywhere, Category = "Event")
	TMap<FString, int> EventIndexes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	bool IsManualEvent;
	UPROPERTY(EditAnywhere, Category = "Event")
	bool IsCompletedEvent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Event")
	bool IsOngoingEvent;
	UPROPERTY(VisibleAnywhere, Category = "Event")
	bool IsOnActive;
	void CompleteTrigger();
private:
	UPROPERTY(EditAnywhere, Category = "Event")
	bool IsFirstTrigger;
	int EventCount;
	int CurrentEventCount;
	bool LoadData();
	virtual void SetActorToSavableList_Implementation() override;
	virtual FString JsonSerialize_Implementation() override;
	virtual bool JsonDeserialize_Implementation() override;
	void SetEventIndexes();
	void CreateMarkerWidget();
	bool IsEventQueueEmpty();
	void ExecuteEvents();
	void UpdateEventCount();
	void UpdateEventState();
	void AddEventCount();
	inline bool IsCurrentEventsCompleted();
	typedef void(AEventActor::* EventFunction)();
	TMap<FString, EventFunction> EventMap;
	UPROPERTY(EditAnywhere, Category = "Event")
	TArray<FEventStruct> EventSequence;
	TArray<TArray<EventFunction>> Events;
	UPROPERTY(EditAnywhere, Category = "Event")
	TArray<AEventActor*> GroupedTriggers;
	void SetEventMaps();
	void SetEventFunctions();
	bool IsValidEventIndex(const int& Sequence);
	bool IsDialogueEvent(const FString& Name);
	int GetLastEventSequence();
	int GetLastDialogueSequence();
	void EnableTrigger();
	void DisableTrigger();
	void StopTimers();
	//UPROPERTY(EditAnywhere, Category = "Event")
	//TArray<EventFunction> Events; 
	//TArray<TArray<EventDelegate>> Events;
	//TArray<EventDelegate> Events;
	//TArray<std::function<void()>> Events; 
public:
	virtual void OnInteract_Implementation(AMyCharacter* Target) override;
	virtual void OnInteractRangeEntered_Implementation(class AMyCharacter* Target) override;
	virtual void OnInteractRangeExited_Implementation(class AMyCharacter* Target) override;
	bool GetOverlapEventState();

//임시
public:
	void PlayInteractionAnimation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Interacation", ScriptName = "Interaction"))
	void K2_PlayInteractionAnimation();
	virtual void K2_PlayInteractionAnimation_Implementation() {};

#pragma region Dialogue
protected:
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	bool IsSeqBasedOnDialogue;
	//UPROPERTY(EditAnywhere, Category = "Dialogue")
	//bool IsInstInteraction;
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<FString> InitialRowName;
	TSharedPtr<TQueue<TSharedPtr<DialogueData>>> DialogueDatas;
	TArray<int> ContDialogueCount;
	float DialogueTimer;
	int DialogueCount;
	void LoadDialogues();
	inline FDialogueData* GetDialogueData(FString RowName);
	inline void EnqueueDialogueData(const TSharedPtr<DialogueData> Data);
	inline bool IsHavingNextDialogue(const FDialogueData* Data);
	void ExecuteDialogue();
	void UpdateDialogueWidget(const TSharedPtr<DialogueData> Data);
	inline bool IsQueueEmpty();
	inline void HideDialogueWidget();
	FTimerHandle DialogueTimerHandle;
public:
#pragma endregion
#pragma region Execution
	//UPROPERTY(EditAnywhere, Category = "Execution")
	//bool IsUsingExecution;
	UPROPERTY(EditAnywhere, Category = "Execution")
	TArray<class AActor*> ExecutionEvents;
	void ExecuteExecutionEvent();

#pragma endregion 
#pragma region Level transition
protected:
	//UPROPERTY(EditAnywhere, Category = "Level transition")
	//bool IsUsingLevelTrans;
	UPROPERTY(EditAnywhere, Category = "Level transition")
	FString LevelRef;
	inline void OpenLevel();
#pragma endregion Save
	UPROPERTY(EditAnywhere, Category = "Save")
	bool IsSavePoint;
	void SaveGame();
	UPROPERTY(EditAnywhere, Category = "Save")
	AActor* SaveLocation;
#pragma region 
#pragma region Marker
	UPROPERTY(EditAnywhere, Category = "Marker")
	AEventActor* Destination;
	UPROPERTY(EditAnywhere, Category = "Marker")
	TArray<AEventActor*> DestinationGroup;	
	UPROPERTY(EditAnywhere, Category = "Marker")
	bool DisplayMarkerOnBeginPlay;
	void SetDestinationMarker();
	UPROPERTY(EditAnywhere, Category = "Marker")
	class UWidgetComponent* Widget;
public:
	//UPROPERTY(EditAnywhere, Category = "Marker")
	//bool IsUsingMarker;
	UPROPERTY(EditAnywhere, Category = "Marker")
	bool IsDestination;
	UPROPERTY(EditAnywhere, Category = "Marker")
	bool IsDisplayingMarker;
	UPROPERTY(EditAnywhere, Category = "Marker")
	TSubclassOf<class UMarkerWidget> WidgetClass;
	UPROPERTY(EditAnywhere, Category = "Marker")
	bool IsMarkerActived;
	void ActivateMarker();
	void SetMarkerVisibility(bool Visib);
	void RemoveMarker();
	void UpdateDistance();
#pragma endregion
#pragma region Timer
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	//bool IsUsingTimer;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool IsUsingWarning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float InitialTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int FirstLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int FinalLimit;
	void ExecuteTimer();
	void TimerWaiter();
	void ExecuteLimitTimer();
	void TimeOver();
	FTimerHandle TimerHandle;
	FTimerHandle LimitTimerHandle;
#pragma endregion
#pragma region NightVision
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NightVision")
	//bool IsUsingNightVision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NightVision")
	bool NightVisionMode;
	void ExecuteNightVisionEvent();
#pragma endregion
};
