using UnityEngine;
using UnityEngine.InputSystem;

public class TitleManager : MonoBehaviour
{
    void Update()
    {
        if (Touchscreen.current == null)
            return;

        Debug.Log("Touch");
        var touch = Touchscreen.current.primaryTouch;

        // タッチ開始
        if (touch.press.wasPressedThisFrame)
        {
            SceneChanger.SceneChange_MainGame();
        }
    }
}
