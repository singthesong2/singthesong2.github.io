using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PoliceCarCtrl : MonoBehaviour
{
    private GameManager gameManager;

    public float CrashDistance = 5.0f;

    public Vector3 PoliceCarSize;

    IEnumerator PoliceFCWS;
    IEnumerator PoliceSiren;

    public void Initialize(GameManager manager)
    {
        gameManager = manager;
    }

    void Awake()
    {
        if (gameManager == null)
            gameManager = GameManager.Instance;
    }

    void Start()
    {
        BoxCollider PoliceCarCollider = GetComponent<BoxCollider>();

        if (PoliceCarCollider != null)
            PoliceCarSize = PoliceCarCollider.size;
    }

    void OnEnable()
    {
        StopAllCoroutines();

        if (gameManager == null)
            return;

        PoliceSiren = Siren();
        PoliceFCWS = PoliceHorn();

        StartCoroutine(PoliceSiren);
        StartCoroutine(PoliceFCWS);
    }

    IEnumerator Siren()
    {
        while (true)
        {
            Debug.Log("»ß¿ë»ß¿ë!");
            yield return new WaitForSeconds(0.5f);
        }
    }

    IEnumerator PoliceHorn()
    {
        while (true)
        {
            Vector3 origin = transform.position;
            Vector3 direction = transform.forward;
            Quaternion rotation = transform.rotation;

            RaycastHit hit;

            if (Physics.BoxCast(origin, PoliceCarSize / 2, direction, out hit, rotation, CrashDistance))
            {
                if (hit.collider.CompareTag("Player"))
                    gameManager.PoliceDecreaseLife();
            }
            yield return new WaitForSeconds(0.1f);
        }
    }
}