using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyGizmos : MonoBehaviour
{
    public Color _color;

    public float radius = 0.1f;

    private void OnDrawGizmos()
    {
        Gizmos.color = _color;

        Gizmos.DrawSphere(transform.position, radius);
    }
}