using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using TMPro;

public class FirstUIManager : MonoBehaviour
{
    public static FirstUIManager firstuimanager { get; private set; }

    void Awake()
    {
        if (firstuimanager != null && firstuimanager != this)
        {
            DestroyImmediate(gameObject);
            return;
        }

        firstuimanager = this;

        DontDestroyOnLoad(gameObject);
    }

    public void OnClickGameStart()
    {
        SceneManager.LoadScene("Play");
        DestroyImmediate(gameObject);
    }

    public void OnClickGameExit()
    {
        #if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
        #else
                Application.Quit();
        #endif
    }
}