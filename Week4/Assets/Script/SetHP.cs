using UnityEngine;
using UnityEngine.UI;

public class SetHP : MonoBehaviour
{
    enum Type
    {
        Player,
        Enemy,
    }
    [SerializeField]
    Type type;

    Image image;

    void Awake()
    {
        image = GetComponent<Image>();
    }

    void Update()
    {
        switch(type)
        {
            case Type.Player:
                image.fillAmount = StatusManager.pHP * 100;
                break;
            case Type.Enemy:
                image.fillAmount = StatusManager.eHP * 100;
                break;
        }
    }
}
