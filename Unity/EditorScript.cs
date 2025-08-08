using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using System.IO;

public class GameObjectLoaderEditor : EditorWindow
{
    private string loadFilePath = "Assets/Editor/scene.level";
    private string saveFilePath = "Assets/Editor/scene.level";
    private GameObjectList gameObjectList = new GameObjectList();

    [MenuItem("Tools/Scene Editor Level Loader")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow<GameObjectLoaderEditor>("Scene Editor Level Loader");
    }

    private void OnGUI()
    {
        loadFilePath = EditorGUILayout.TextField("Load File Path", loadFilePath);

        if (GUILayout.Button("Load"))
            LoadLevel(loadFilePath);
        

        saveFilePath = EditorGUILayout.TextField("Save File Path", saveFilePath);

        if (GUILayout.Button("Save"))
            SaveLevel(saveFilePath);
        
    }
    private void LoadLevel(string path)
    {
        if (File.Exists(path))
        {
            string json = File.ReadAllText(path);

            gameObjectList = JsonUtility.FromJson<GameObjectList>(json);

            Camera c;
            Light l;
            foreach (GameObject obj in Object.FindObjectsOfType<GameObject>())
            {
                if (!obj.TryGetComponent<Camera>(out c) && !obj.TryGetComponent<Light>(out l))
                    DestroyImmediate(obj);
            }

            foreach (var data in gameObjectList.GameObjects)
            {
                GameObject obj = null;

                if(data.objType == 0) //2 is for Mesh Objects
                    obj = GameObject.CreatePrimitive(PrimitiveType.Cube);
                else if (data.objType == 1)
                    obj = GameObject.CreatePrimitive(PrimitiveType.Plane);
                else if (data.objType == 3)
                    obj = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                else if (data.objType == 4)
                    obj = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
                else if (data.objType == 5)
                    obj = GameObject.CreatePrimitive(PrimitiveType.Capsule);

                if (obj != null)
                {
                    obj.name = data.name;
                    obj.transform.position = new Vector3(data.position.px, data.position.py, data.position.pz);
                    obj.transform.localScale = new Vector3(data.scale.sx, data.scale.sy, data.scale.sz);
                    obj.transform.eulerAngles = new Vector3(data.rotation.rx, data.rotation.ry, data.rotation.rz);

                    if (data.components.Contains(3))
                    {
                        Rigidbody rBody = obj.AddComponent<Rigidbody>();

                        if (data.objType != 1)
                        {
                            rBody.useGravity = true;
                            rBody.isKinematic = false;
                            rBody.mass = 100;
                        }
                        else
                        {
                            rBody.isKinematic = true;
                        }
                    }

                }
            }
        }
        else
        {
            Debug.Log("JSON file not found!");
        }
    }

    private void SaveLevel(string path)
    {
        Camera c;
        Light l;

        gameObjectList.GameObjects = new List<GameObjectData>();
        gameObjectList.GameObjects.Clear();

        foreach (GameObject obj in Object.FindObjectsOfType<GameObject>())
        {
            if (!obj.TryGetComponent<Camera>(out c) && !obj.TryGetComponent<Light>(out l))
            {
                int primitive = 2;
                if (obj.GetComponent<MeshFilter>() != null)
                {
                    if (obj.GetComponent<MeshFilter>().sharedMesh.name == "Cube") //2 is for Mesh Objects
                        primitive = 0;
                    else if (obj.GetComponent<MeshFilter>().sharedMesh.name == "Plane")
                        primitive = 1;
                    else if (obj.GetComponent<MeshFilter>().sharedMesh.name == "Sphere")
                        primitive = 3;
                    else if (obj.GetComponent<MeshFilter>().sharedMesh.name == "Cylinder")
                        primitive = 4;
                    else if (obj.GetComponent<MeshFilter>().sharedMesh.name == "Capsule")
                        primitive = 5;
                }

                List<int> component = new List<int>();
                if (obj.GetComponent<Rigidbody>() != null)
                    component.Add(3);

                GameObjectData data = new GameObjectData
                {
                    name = obj.name,
                    objType = primitive,
                    position = new Position { px = obj.transform.position.x, py = obj.transform.position.y, pz = obj.transform.position.z },
                    scale = new Scale { sx = obj.transform.localScale.x, sy = obj.transform.localScale.y, sz = obj.transform.localScale.z },
                    rotation = new Rotation { rx = obj.transform.eulerAngles.x, ry = obj.transform.eulerAngles.y, rz = obj.transform.eulerAngles.z },
                    components = component,
                    texture = "null"
                };

                gameObjectList.GameObjects.Add(data);
            }
        }

        string json = JsonUtility.ToJson(gameObjectList, true);

        File.WriteAllText(path, json);
        Debug.Log("Level saved to " + path);
    }
}


