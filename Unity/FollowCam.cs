using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowCam : MonoBehaviour
{
    public Transform targetTr;

    private Transform camTr;

    Vector3 camdir;

    Vector3 Offset;

    Vector3 Velocity;

    [Range(1.0f, 20.0f)]
    public float targetOffset = 2.0f;

    [Range(0.0f, 10.0f)]
    public float height = 4.0f;

    void Awake()
    {
        camTr = GetComponent<Transform>();

        camdir = targetTr.transform.position - camTr.transform.position;
    }

    void LateUpdate()
    {
        Velocity = Vector3.zero;

        Offset = new Vector3(targetOffset, 3.0f, -1.0f);

        camTr.position = targetTr.position + (-camdir) + (Vector3.up * height) + Offset;

        camTr.LookAt(targetTr.position);
    }
}