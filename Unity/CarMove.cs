using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarMove : MonoBehaviour
{
    private GameManager gameManager;

    public float CarSpeed = 10.0f;

    public Transform DeleteCarPoint;

    public float CrashDistance = 5.0f;

    public Vector3 CarSize;

    IEnumerator CMove;
    IEnumerator FCWS;

    public void Initialize(GameManager manager, Transform deletePoint)
    {
        gameManager = manager;
        DeleteCarPoint = deletePoint;
    }

    public void CarSpeedOption(float speedopt)
    {
        CarSpeed = speedopt;
    }

    void Awake()
    {
        if (gameManager == null)
            gameManager = GameManager.Instance;

        if (DeleteCarPoint == null)
            DeleteCarPoint = GameManager.Instance?.deleteCarPoint;
    }

    void Start()
    {
        BoxCollider carCollider = GetComponent<BoxCollider>();
        if (carCollider != null)
        {
            CarSize = carCollider.size;
        }
    }

    void OnEnable()
    {
        StopAllCoroutines();

        CMove = carmove();
        FCWS = horn();

        if (gameObject.layer != LayerMask.NameToLayer("Police"))
            StartCoroutine(FCWS);

        StartCoroutine(CMove);
    }

    void OnDisable()
    {
        StopAllCoroutines();
    }

    IEnumerator carmove()
    {
        while (true)
        {
            transform.Translate(Vector3.forward * CarSpeed * Time.deltaTime);

            if (DeleteCarPoint != null && Vector3.Distance(transform.position, DeleteCarPoint.position) < 1.0f)
            {
                if (gameManager != null)
                    gameManager.DeactivateCar(gameObject);

                yield break;
            }
            yield return null;
        }
    }

    IEnumerator horn()
    {
        while (true)
        {
            Vector3 origin = transform.position;
            Vector3 direction = transform.forward;
            Quaternion rotation = transform.rotation;

            RaycastHit hit;

            if (Physics.BoxCast(origin, CarSize / 2, direction, out hit, rotation, CrashDistance))
            {
                if (hit.collider.CompareTag("Player"))
                {
                    Debug.Log("»§»§!");
                }
            }
            yield return new WaitForSeconds(0.1f);
        }
    }

    void OnCollisionEnter(Collision coll)
    {
        if (coll.collider.CompareTag("Player"))
        {
            if (gameManager != null)
            {
                gameManager.DeactivateCar(gameObject);
                gameManager.DecreaseLife(1);
            }
        }
    }
}