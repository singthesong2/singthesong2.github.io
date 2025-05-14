using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCtrl : MonoBehaviour
{
    private GameManager gameManager;

    private Transform tr;

    private bool IsMove = false;

    public int MoveMaxCount = 27;

    public int MoveCount = 0;

    IEnumerator IEMove;

    void Start()
    {
        tr = GetComponent<Transform>();

        if (gameManager == null)
            gameManager = GameManager.Instance;

        IEMove = Move();

        StartCoroutine(IEMove);
    }

    IEnumerator Move()
    {
        while (true)
        {
            if (!IsMove)
            {
                if (Input.GetKeyDown(KeyCode.W))
                    TryMove(Vector3.forward);
                else if (Input.GetKeyDown(KeyCode.A))
                    TryMove(Vector3.left);
                else if (Input.GetKeyDown(KeyCode.D))
                    TryMove(Vector3.right);
            }
            yield return null;
        }
    }

    void TryMove(Vector3 dir)
    {
        StartCoroutine(MoveAndRotate(dir));

        if (dir == Vector3.forward)
            PlusSpeed();
    }

    IEnumerator MoveAndRotate(Vector3 dir)
    {
        if (TreeCheck(dir))
            yield break;

        IsMove = true;

        Quaternion targetRotation = Quaternion.LookRotation(dir);
        tr.rotation = Quaternion.Slerp(tr.rotation, targetRotation, 1.0f);
        Vector3 LimitPosition = tr.position + dir;
        LimitPosition.x = Mathf.Clamp(LimitPosition.x, -4f, 20f);

        tr.position = LimitPosition;

        yield return new WaitForSeconds(0.1f);

        IsMove = false;
    }

    bool TreeCheck(Vector3 dir)
    {
        float distance = 1.0f;
        float radius = 0.3f;
        float height = 1.0f;

        Vector3 Start = transform.position + Vector3.up * (height / 2 - radius);
        Vector3 End = tr.position + Vector3.up * (height / 2 + radius);

        RaycastHit hit;

        if(Physics.CapsuleCast(Start, End, radius, dir, out hit, distance, LayerMask.GetMask("Tree")))
            return true;

        return false;
    }

    void PlusSpeed()
    {
        gameManager.IncreaseScore(1);
        MoveCount++;

        if (MoveCount >= MoveMaxCount)
        {
            gameManager.IncreaseSpeed(5.0f);
            MoveCount = 0;
        }
    }
}