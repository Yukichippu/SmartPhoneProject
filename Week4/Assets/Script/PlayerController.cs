using System.Collections;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerController : MonoBehaviour
{
    public static PlayerController Inst_PlayerController;

    Animator myAnimator;
    BoxCollider myCollider;

    [SerializeField] BoxCollider myHitBox;

    Vector2 startPos;
    [SerializeField] 
    float swipeDistance = 100f;

    void Awake()
    {
        Inst_PlayerController = this;

        StatusManager.ResetHP();
        myAnimator = GetComponent<Animator>();
        myCollider = GetComponent<BoxCollider>();

        myHitBox.enabled = false;
    }

    void Update()
    {
        if (Touchscreen.current == null)
            return;

        var touch = Touchscreen.current.primaryTouch;

        // タッチ開始
        if (touch.press.wasPressedThisFrame)
        {
            startPos = touch.position.ReadValue();
        }

        // タッチ終了
        if (touch.press.wasReleasedThisFrame)
        {
            Vector2 endPos = touch.position.ReadValue();

            // デバッグ用の線を5秒間表示
            Debug.DrawLine(
                new Vector3(startPos.x, startPos.y, 0),
                new Vector3(endPos.x, endPos.y, 0),
                Color.red,
                5f);

            Vector2 swipe = endPos - startPos;

            if (swipe.magnitude >= swipeDistance)
            {
                Debug.Log("攻撃！（スワイプ）");
                StartCoroutine(Attack());
            }
            else
            {
                Debug.Log("防御！（タップ）");
                //myAnimator.Play("IdleCombat");
            }
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (LayerMask.LayerToName(other.gameObject.layer) == "Weapons")
        {
            StatusManager.Damaged_P();
            myAnimator.Play("GetHit");
        }
    }

    IEnumerator Attack()
    {
        myAnimator.Play("MeleeAttack_OneHanded");
        myHitBox.enabled = true;

        yield return new WaitForSeconds(0.5f);

        myHitBox.enabled = false;

        yield return null;
    }
    public IEnumerator Rolling(bool b)
    {
        float t = 0;
        float add = 3.0f;
        Vector3 startPos = transform.position;
        Vector3 endPos = transform.position;

        if (b)
        {
            endPos = new Vector3(
                startPos.x - add,
                startPos.y,
                startPos.z
                );
            myAnimator.Play("RollLeft");

            while (t < 1)
            {
                t += Time.deltaTime;
                transform.position = Vector3.Lerp( startPos, endPos, t );
            }
        }
        else
        {
            endPos = new Vector3(
                startPos.x + add,
                startPos.y,
                startPos.z
                );
            myAnimator.Play("RollRight");

            while (t < 1)
            {
                t += Time.deltaTime;
                transform.position = Vector3.Lerp(startPos, endPos, t);
            }
        }
        t = 0;
        
        myCollider.enabled = false;

        yield return new WaitForSeconds(0.5f);

        myCollider.enabled = true;

        yield return null;
    }
}
