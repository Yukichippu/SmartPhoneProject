using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class EnemyController : MonoBehaviour
{
    Animator myAnimator;
    [SerializeField] List<SkinnedMeshRenderer> mySkinnedMeshRenderer = new();
    [SerializeField] BoxCollider myHitBox;

    List<Color> defColor = new(); 

    float time;
    float waitTime;

    bool attacking;
    bool dead;

    void Awake()
    {
        myAnimator = GetComponent<Animator>();

        waitTime = Random.Range(1.0f, 5.0f);
        for(int i = 0; i < mySkinnedMeshRenderer.Count; i++)
        {
            defColor.Add(mySkinnedMeshRenderer[i].material.color);
        }

        attacking = false;
        dead = false;
        myHitBox.enabled = false;
    }

    void Update()
    {
        if (StatusManager.eHP <= 0 && !dead)
        {
            StartCoroutine(Death());
        }

        time += Time.deltaTime;
        if(time >= waitTime && !attacking && !dead)
        {
            StartCoroutine(Attack());
        }
    }

    void OnTriggerEnter(Collider other)
    {
        Debug.Log("HIT");

        if (LayerMask.LayerToName(other.gameObject.layer) == "Weapons")
        {
            StatusManager.Damaged_E();
            StartCoroutine(BlinkColor());
            myAnimator.Play("root|Take Damage");
        }
    }

    IEnumerator Attack()
    {
        attacking = true;

        myAnimator.Play("root|scream");

        yield return new WaitForSeconds(1.5f);

        myAnimator.Play("root|slash 02");

        yield return new WaitForSeconds(0.5f);

        myHitBox.enabled = true;

        yield return new WaitForSeconds(0.1f);

        myHitBox.enabled = false;

        yield return new WaitForSeconds(0.9f);

        waitTime = Random.Range(1.0f, 5.0f);
        time = 0;
        attacking = false;
        yield return null;
    }

    IEnumerator Death()
    {
        myAnimator.Play("root|death");
        dead = true;

        yield return new WaitForSeconds(1.5f);

        SceneChanger.SceneChange_Result();

        yield return null;
    }

    IEnumerator BlinkColor()
    {
        for (int i = 0; i < defColor.Count; i++)
        {
            mySkinnedMeshRenderer[i].material.color = Color.red;
        }

        yield return new WaitForSeconds(0.1f);

        for (int i = 0; i < defColor.Count; i++)
        {
            mySkinnedMeshRenderer[i].material.color = defColor[i];
        }

        yield return null;
    }
}
