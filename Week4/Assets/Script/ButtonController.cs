using UnityEngine;

public class ButtonController : MonoBehaviour
{
    public void OnLeftRolling()
    {
        StartCoroutine(PlayerController.Inst_PlayerController.Rolling(true));
        Debug.Log("Called");
    }

    public void OnRightRolling()
    {
        StartCoroutine(PlayerController.Inst_PlayerController.Rolling(false));
    }
}
