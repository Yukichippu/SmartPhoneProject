using UnityEngine;

public class StatusManager
{
    public static float pHP = 1f;
    public static float eHP = 1f;

    public static void ResetHP()
    {
        pHP = 1f;
        eHP = 1f;
    }

    public static void Damaged_P()
    {
        pHP -= 0.2f;
    }
    public static void Damaged_E()
    {
        eHP -= 0.2f;
    }
}
