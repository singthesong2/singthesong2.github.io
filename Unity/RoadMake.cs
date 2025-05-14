using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoadMake : MonoBehaviour
{
    private GameManager gameManager;

    public GameObject StartRoad;
    public GameObject CarRoad;
    public GameObject TreeRoad;

    public Transform Player;
    public int MaxRoadCount = 18;
    public float RoadLen = 3.0f;
    private int RoadCycleIndex = 1;

    private Queue<GameObject> QueueRoad = new Queue<GameObject>();

    private Dictionary<GameObject, Coroutine> CarRoadDic = new Dictionary<GameObject, Coroutine>();

    private Vector3 NextRoadPosition;

    public Transform SpawnPosition;

    IEnumerator CheckSpawnRoad;

    void Awake()
    {
        if (gameManager == null)
            gameManager = GameManager.Instance;
    }

    void Start()
    {
        Vector3 StartRoadPosition = Vector3.zero;
        Instantiate(StartRoad, StartRoadPosition, Quaternion.identity);

        NextRoadPosition = StartRoad.transform.position + Vector3.forward * RoadLen;

        InitializeRoad();

        CheckSpawnRoad = CSRoad();
        StartCoroutine(CheckSpawnRoad);
    }

    void InitializeRoad()
    {
        for (int i = 0; i < 9; i++)
        {
            if(i==2||i==5||i==8)
                SpawnRoad(TreeRoad);
            else
                SpawnRoad(CarRoad);
        }
    }

    void SpawnRoad(GameObject Road)
    {
        Quaternion roadRotation = Quaternion.identity;

        bool IsRotated = false;

        if (Road == CarRoad)
        {
            if ((RoadCycleIndex % 3) == 2)
            {
                roadRotation = Quaternion.Euler(0, 180, 0);
                IsRotated = true;
            }
        }

        GameObject NewRoad = Instantiate(Road, NextRoadPosition, roadRotation);
        QueueRoad.Enqueue(NewRoad);

        if (Road == CarRoad)
        {
            Transform spawnPoint = NewRoad.transform.Find("CarSpawnPointGroup/SpawnCarPoint");
            Transform deletePoint = NewRoad.transform.Find("CarSpawnPointGroup/DeleteCarPoint");

            if (spawnPoint != null && deletePoint != null)
            {
                Coroutine carCoroutine = StartCoroutine(gameManager.StartCar(NewRoad, spawnPoint, deletePoint, IsRotated));
                CarRoadDic[NewRoad] = carCoroutine;
            }

            gameManager.CreateCar(NewRoad, deletePoint, IsRotated);
        }
        else if (Road == TreeRoad)
            gameManager.CreateTree(NewRoad);

        NextRoadPosition += Vector3.forward * RoadLen;

        RoadCycleIndex++;
    }

    void DeleteRoad()
    {
        while (QueueRoad.Count > 0)
        {
            GameObject road = QueueRoad.Peek();
            if (road.transform.position.z < Player.position.z - 6.0f)
            {
                QueueRoad.Dequeue();

                if (CarRoadDic.ContainsKey(road))
                {
                    StopCoroutine(CarRoadDic[road]);
                    CarRoadDic.Remove(road);
                }

                gameManager.RemoveTree(road);

                gameManager.RemoveCar(road);

                Destroy(road);
            }
            else
            {
                break;
            }
        }
    }

    IEnumerator CSRoad()
    {
        while (true)
        {
            if (Player.position.z >= NextRoadPosition.z - (MaxRoadCount * RoadLen / 2))
            {
                if (RoadCycleIndex % 3 == 0)
                    SpawnRoad(TreeRoad);
                else
                    SpawnRoad(CarRoad);

                DeleteRoad();
            }
            yield return null;
        }
    }
}