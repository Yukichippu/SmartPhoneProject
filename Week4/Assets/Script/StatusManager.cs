using UnityEngine;

public class StatusManager
{
    public static float pHP = 100f;
    public static float eHP = 100f;

    public static void ResetHP()
    {
        pHP = 100f;
        eHP = 100f;
    }

    public static void Damaged_P()
    {
        pHP -= 20f;
    }
    public static void Damaged_E()
    {
        eHP -= 20f;
    }
}
