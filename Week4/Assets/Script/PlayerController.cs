using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerController : MonoBehaviour
{
    Animator myAnimator;

    Vector2 startPos;
    [SerializeField] 
    float swipeDistance = 100f;

    private void Awake()
    {
        StatusManager.ResetHP();
        StatusManager.Damaged_P();
        StatusManager.Damaged_E();

        myAnimator = GetComponent<Animator>();
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
                myAnimator.Play("MeleeAttack_OneHanded");
            }
            else
            {
                Debug.Log("防御！（タップ）");
                myAnimator.Play("IdleCombat");
            }
        }
    }

    private void OnCollisionEnter(Collision coll)
    {
        if (coll.transform.tag == "Weapons")
        {
            StatusManager.pHP -= 20f;
        }
    }
}
