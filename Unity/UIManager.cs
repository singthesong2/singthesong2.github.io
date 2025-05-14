using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;
using UniRx;

public class UIManager : MonoBehaviour
{
    public static UIManager uimanager { get; private set; }

    private GameManager gameManager;

    public GameObject GameCanvas;
    public GameObject GameOverCanvas;

    public TMP_Text ScoreText;
    public TMP_Text LifeText;

    public Button ReStartButton;
    public Button ExitButton;

    private PlayerViewModel playerViewModel;

    void Awake()
    {
        if (uimanager != null && uimanager != this)
        {
            DestroyImmediate(gameObject);
            return;
        }

        uimanager = this;

        SceneManager.sceneLoaded += OnSceneLoaded;
    }

    void Start()
    {
        if (gameManager == null)
            gameManager = GameManager.Instance;

        playerViewModel = gameManager.PlayerViewModel;

        if (playerViewModel == null)
            return;

        playerViewModel.Score
            .Subscribe(score =>
            {
                ScoreText.text = $"<b><color=black>Score : {score}";
            })
            .AddTo(this);

        playerViewModel.Life
            .Subscribe(life =>
            {
                LifeText.text = $"<b><color=red>Life : {life}";

                if (life <= 0)
                {
                    playerViewModel.GameOverCanvasOnOff(true);
                    playerViewModel.ReStartButtonOnOff(true);
                    playerViewModel.ExitButtonOnOff(true);
                }
            })
            .AddTo(this);

        playerViewModel.IsGameOverCanvasActive
            .Subscribe(isbool =>
            {
                GameOverCanvas.SetActive(isbool);
                ReStartButton.gameObject.SetActive(isbool);
                ExitButton.gameObject.SetActive(isbool);
            })
            .AddTo(this);
    }

    public void OnClickReStart()
    {
        var gm = GameManager.Instance;

        if (gm != null)
        {
            gm.ReSetGame();

            if (gm.PlayerViewModel != null)
            {
                gm.ReSetPlayerViewModel();
                gm.PlayerViewModel.Life.Value = 3;
                gm.PlayerViewModel.Score.Value = 0;
                gm.PlayerViewModel.IsGameOverCanvasActive.Value = false;
            }

            Destroy(gm.gameObject);
        }
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }


    public void OnClickExit()
    {
        var gm = GameManager.Instance;

        if (gm != null)
        {
            gm.ReSetGame();

            if (gm.PlayerViewModel != null)
            {
                gm.ReSetPlayerViewModel();
                gm.PlayerViewModel.Life.Value = 3;
                gm.PlayerViewModel.Score.Value = 0;
                gm.PlayerViewModel.IsGameOverCanvasActive.Value = false;
            }

            Destroy(gm.gameObject);
        }
        SceneManager.LoadScene("FirstUI");
        Destroy(gameObject);
    }

    public void ReSetUI()
    {
        GameCanvas = GameObject.Find("GameCanvas");
        GameOverCanvas = GameObject.Find("GameOverCanvas");

        ReStartButton = GameObject.Find("ReStartButton").GetComponent<Button>();
        ExitButton = GameObject.Find("ExitButton").GetComponent<Button>();

        var scoreObj = GameObject.Find("ScoreText");
        var lifeObj = GameObject.Find("LifeText");

        var restartbuttonObj = GameObject.Find("ReStartButton");
        var exitbuttonObj = GameObject.Find("ExitButton");

        if (scoreObj != null) ScoreText = scoreObj.GetComponent<TMP_Text>();
        if (lifeObj != null) LifeText = lifeObj.GetComponent<TMP_Text>();
        if (restartbuttonObj != null) ReStartButton = restartbuttonObj.GetComponent<Button>();
        if (exitbuttonObj != null) ExitButton = exitbuttonObj.GetComponent<Button>();

        if (GameCanvas == null || GameOverCanvas == null || ScoreText == null || LifeText == null)
        {
            Debug.LogError("UIManager: UI 요소가 제대로 연결되지 않았습니다.");
        }
        else
        {
            var gm = GameManager.Instance;

            if (gm != null && gm.PlayerViewModel != null)
                BindingViewModel(gm.PlayerViewModel);
        }
    }

    public void BindingViewModel(PlayerViewModel vm)
    {
        if (vm == null)
            return;

        vm.Score
            .Subscribe(score =>
            {
                ScoreText.text = $"<b><color=black>Score : {score}";
            })
            .AddTo(this);

        vm.Life
            .Subscribe(life =>
            {
                LifeText.text = $"<b><color=red>Life : {life}";

                if (life <= 0)
                {
                    vm.GameOverCanvasOnOff(true);
                    vm.ReStartButtonOnOff(true);
                    vm.ExitButtonOnOff(true);
                }
            })
            .AddTo(this);

        vm.IsGameOverCanvasActive
            .Subscribe(isbool =>
            {
                GameOverCanvas.SetActive(isbool);
                ReStartButton.gameObject.SetActive(isbool);
                ExitButton.gameObject.SetActive(isbool);
            })
            .AddTo(this);
    }

    void OnDestroy()
    {
        SceneManager.sceneLoaded -= OnSceneLoaded;
    }

    void OnSceneLoaded(Scene scene, LoadSceneMode mode)
    {
        ReSetUI();
    }
}