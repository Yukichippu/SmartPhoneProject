using UnityEngine;

public class PlayerController : MonoBehaviour
{
    enum State
    {
        Idle,
        Walk,
        Attack,
        Guard,
    }
    State state;

    void StateController()
    {
        switch(state)
        {
            case State.Idle:
                break;
            case State.Walk:
                break;
            case State.Attack:
                break;
            case State.Guard:
                break;
        }
    }

    void Walk()
    {
        float pPosX = Time.deltaTime * 5.0f;

        transform.position = new Vector3(
            pPosX,
            transform.position.y,
            transform.position.z
            );
    }


}
