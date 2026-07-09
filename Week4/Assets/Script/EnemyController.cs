using UnityEngine;

public class EnemyController : MonoBehaviour
{
    Animator myAnimator;
    [SerializeField] BoxCollider myHitBox;

    bool dead;

    void Awake()
    {
        myAnimator = GetComponent<Animator>();

        dead = false;
        myHitBox.enabled = false;
    }

    void Update()
    {
        if (StatusManager.eHP <= 0 && !dead)
        {
            myAnimator.Play("root|death");
            dead = true;
        }
    }

    void OnTriggerEnter(Collider other)
    {
        Debug.Log("HIT");

        if (LayerMask.LayerToName(other.gameObject.layer) == "Weapons")
        {
            StatusManager.Damaged_E();
            myAnimator.Play("root|Take Damage");
        }
    }
}
