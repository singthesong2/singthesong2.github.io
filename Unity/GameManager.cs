using System.Collections;
using System.Collections.Generic;
using System.Linq;
#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance { get; private set; }

    public Dictionary<string, Transform> points = new Dictionary<string, Transform>();

    public List<Transform> treepoints = new List<Transform>();

    private Dictionary<GameObject, List<GameObject>> RoadTree= new Dictionary<GameObject, List<GameObject>>();

    private Dictionary<GameObject, List<GameObject>> RoadCar = new Dictionary<GameObject, List<GameObject>>();

    public GameObject tree;

    public Transform spawnCarPoint;
    public Transform deleteCarPoint;
    public List<Transform> carpoints = new List<Transform>();
    public List<GameObject> carPrefabs = new List<GameObject>();

    private List<GameObject> carPooling = new List<GameObject>();
    public int ListPoolingSize = 50;

    public PlayerViewModel PlayerViewModel { get; private set; }
    public PlayerModel PlayerModel { get; private set; }

    public float CarSpeeding = 10.0f;

    public float CarTime = 3.0f;

    void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
             DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }

        PlayerModel = new PlayerModel(0, 3);
        PlayerViewModel = new PlayerViewModel();

        PlayerViewModel.Score.Value = PlayerModel.Score;
        PlayerViewModel.Life.Value = PlayerModel.Life;
    }

    void Start()
    {
        InitializePoint();
        ListCarPooling();
    }

    void InitializePoint()
    {
        Transform treeSpawnPointGroup = GameObject.Find("TreeSpawnPointGroup")?.transform;
        if (treeSpawnPointGroup != null)
        {
            foreach (Transform treepoint in treeSpawnPointGroup)
                treepoints.Add(treepoint);
        }

        Transform carSpawnPointGroup = GameObject.Find("CarSpawnPointGroup")?.transform;
        if (carSpawnPointGroup != null)
        {
            foreach (Transform carpoint in carSpawnPointGroup)
                points[carpoint.name] = carpoint;
        }

        points.TryGetValue("SpawnCarPoint", out spawnCarPoint);
        points.TryGetValue("DeleteCarPoint", out deleteCarPoint);


        HashSet<Transform> CarOnePoints = new HashSet<Transform>(points.Values);
        CarOnePoints.Remove(spawnCarPoint);
        CarOnePoints.Remove(deleteCarPoint);

        carpoints = CarOnePoints.ToList();
    }

    public void CreateTree(GameObject road)
    {
        Transform TreeGroup = road.transform.Find("TreeSpawnPointGroup");

        if (TreeGroup == null)
            return;

        int maxTreeCount = TreeGroup.childCount;
        int spawnCount = Random.Range(0, maxTreeCount + 1);

        List<Transform> TreeSpawnList = new List<Transform>();
        List<GameObject> SpawnTrees = new List<GameObject>();


        foreach (Transform t in TreeGroup)
            TreeSpawnList.Add(t);

        for (int i = 0; i < spawnCount; i++)
        {
            if (TreeSpawnList.Count == 0)
                break;

            int idx = Random.Range(0, TreeSpawnList.Count);
            Vector3 treePos = TreeSpawnList[idx].position;
            treePos.y = -0.1f;

            GameObject newTree = Instantiate(tree, treePos, Quaternion.Euler(0, 270, 0));
            SpawnTrees.Add(newTree);

            TreeSpawnList.RemoveAt(idx);
        }

        RoadTree[road] = SpawnTrees;
    }

    void ListCarPooling()
    {
        for (int i = 0; i < ListPoolingSize; i++)
        {
            int randomIndex = Random.Range(0, carPrefabs.Count);
            GameObject car = Instantiate(carPrefabs[randomIndex]);
            car.SetActive(false);
            carPooling.Add(car);
        }
    }

    void SpawnCarPooling(GameObject road, Vector3 position, Quaternion rotation, Transform deletePoint)
    {
        GameObject car = carPooling.FirstOrDefault(c => !c.activeInHierarchy);

        if (car != null)
        {
            foreach (var kvp in RoadCar)
            {
                if (kvp.Value.Contains(car))
                {
                    kvp.Value.Remove(car);
                    break;
                }
            }

            car.transform.position = position;
            car.transform.rotation = rotation;
            car.SetActive(true);

            CarMove carMove = car.GetComponent<CarMove>();
            if (carMove == null)
                carMove = car.AddComponent<CarMove>();

            carMove.Initialize(this, deletePoint);
            carMove.CarSpeedOption(CarSpeeding);


            if (!RoadCar.ContainsKey(road))
                RoadCar[road] = new List<GameObject>();

            RoadCar[road].Add(car);
        }
    }

    public void CreateCar(GameObject road, Transform deletePoint, bool IsRotated)
    {
        Transform carGroup = road.transform.Find("CarSpawnPointGroup");
        
        if (carGroup == null)
               return;

        Quaternion carRotation = IsRotated ? Quaternion.Euler(0, 270, 0) : Quaternion.Euler(0, 90, 0);

        HashSet<Vector3> UsePosition = new HashSet<Vector3>();

        foreach (Transform point in carGroup)
        {
            if (point.name == "SpawnCarPoint" || point.name == "DeleteCarPoint")
                continue;

            if (!UsePosition.Contains(point.position))
            {
                SpawnCarPooling(road, point.position, carRotation, deletePoint);
                UsePosition.Add(point.position);
            }
        }
    }

    public void DeactivateCar(GameObject car)
    {
        car.SetActive(false);
    }

    public void DecreaseLife(int life)
    {
        PlayerModel.DecreaseLife(life);
        PlayerViewModel.Life.Value = PlayerModel.Life;

        if (PlayerModel.Life == 0)
            GameOver();
    }

    public void PoliceDecreaseLife()
    {
        PlayerModel.PoliceLife();
        PlayerViewModel.Life.Value = PlayerModel.Life;

        GameOver();
    }

    public void IncreaseScore(int score)
    {
        PlayerModel.IncreaseScore(score);
        PlayerViewModel.Score.Value = PlayerModel.Score;
    }

    public void GameOver()
    {
        Time.timeScale = 0f;
    }

    public void CreateCarAt(GameObject road, Vector3 position, Transform deletePoint)
    {
        SpawnCarPooling(road, position, Quaternion.Euler(0, 90, 0), deletePoint);
    }

    public void RemoveTree(GameObject road)
    {
        if (RoadTree.ContainsKey(road))
        {
            foreach (GameObject tree in RoadTree[road])
            {
                if (tree != null)
                    Destroy(tree);
            }

            RoadTree.Remove(road);
        }
    }
       
    public void RemoveCar(GameObject road)
    {
        if (RoadCar.ContainsKey(road))
        {
            foreach (GameObject car in RoadCar[road])
            {
                if (car != null && car.activeInHierarchy)
                    DeactivateCar(car);
            }
            RoadCar.Remove(road);
        }
    }

    public void IncreaseSpeed(float speed)
    {
        CarSpeeding += speed;
        CarTime -= 0.1f;

        if (CarTime < 0.1f)
            CarTime = 0.1f;
    }

    public void ReSetGame()
    {
        StopAllCoroutines();

        foreach (var car in carPooling)
        {
            car.SetActive(false);
        }

        carPooling.Clear();
        RoadCar.Clear();
        RoadTree.Clear();

        Time.timeScale = 1f;
    }
    public void ReSetPlayerViewModel()
    {
        PlayerViewModel = new PlayerViewModel();
        PlayerViewModel.Score.Value = PlayerModel.Score;
        PlayerViewModel.Life.Value = PlayerModel.Life;
    }

    public IEnumerator StartCar(GameObject road, Transform SpawnPoint, Transform DeletePoint, bool IsRotated)
    {
        yield return new WaitForSeconds(CarTime);

        while (true)
        {
            if (SpawnPoint == null || !SpawnPoint.gameObject.activeInHierarchy)
            {
                yield break;
            }

            Quaternion carRotation = IsRotated ? Quaternion.Euler(0, 270, 0) : Quaternion.Euler(0, 90, 0);
            SpawnCarPooling(road, SpawnPoint.position, carRotation, DeletePoint);
            yield return new WaitForSeconds(CarTime);
        }
    }
}