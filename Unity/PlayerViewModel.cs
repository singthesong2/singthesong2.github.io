using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UniRx;

public class PlayerViewModel
{
    public ReactiveProperty<bool> IsGameOverCanvasActive { get; }
    public ReactiveProperty<bool> IsReStartButtonActive { get; }
    public ReactiveProperty<bool> IsExitButtonActive { get; }

    public ReactiveProperty<int> Score { get; private set; }
    public ReactiveProperty<int> Life { get; private set; }

    public PlayerViewModel()
    {
        IsGameOverCanvasActive = new ReactiveProperty<bool>(false);
        IsReStartButtonActive = new ReactiveProperty<bool>(false);
        IsExitButtonActive = new ReactiveProperty<bool>(false);

        Score = new ReactiveProperty<int>(0);
        Life = new ReactiveProperty<int>(3);
    }

    public void GameOverCanvasOnOff(bool isbool)
    {
        IsGameOverCanvasActive.Value = isbool;
    }

    public void ReStartButtonOnOff(bool isbool)
    {
        IsReStartButtonActive.Value = isbool;
    }

    public void ExitButtonOnOff(bool isbool)
    {
        IsExitButtonActive.Value = isbool;
    }
}