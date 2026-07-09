using UnityEngine;

public class EnemyController : MonoBehaviour
{
    private void OnCollisionEnter(Collision coll)
    {
        if (coll.transform.tag == "Weapons")
        {
            StatusManager.eHP -= 20f;
        }
    }
}
