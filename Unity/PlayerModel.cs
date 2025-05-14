using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerModel
{
    public int Score { get; private set; }
    public int Life { get; private set; }

    public PlayerModel(int FirstScore, int FirstLife)
    {
        Score = FirstScore;
        Life = FirstLife;
    }

    public void IncreaseScore(int amount)
    {
        Score += amount;
    }

    public void DecreaseLife(int amount)
    {
        Life = Mathf.Max(Life - amount, 0);
    }

    public void PoliceLife()
    {
        Life = 0;
    }
}