using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneChanger
{
    public static void SceneChange_MainGame()
    {
        SceneManager.LoadScene("MainGame");
    }

    public static void SceneChange_Result()
    {
        SceneManager.LoadScene("Result");
    }
}
