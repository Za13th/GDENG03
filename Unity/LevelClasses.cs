using System.Collections.Generic;

[System.Serializable]
public class Position
{
    public float px, py, pz;
}

[System.Serializable]
public class Rotation
{
    public float rx, ry, rz;
}

[System.Serializable]
public class Scale
{
    public float sx, sy, sz;
}

[System.Serializable]
public class GameObjectData
{
    public List<int> components;
    public string name;
    public int objType;
    public Position position;
    public Rotation rotation;
    public Scale scale;
    public string texture;
}

[System.Serializable]
public class GameObjectList
{
    public List<GameObjectData> GameObjects;
}
