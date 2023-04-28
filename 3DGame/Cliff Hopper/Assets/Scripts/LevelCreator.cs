using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum TerrainType {Button, Plain, Stair};
public class LevelCreator : MonoBehaviour
{
    public GameObject buttonPrefab, plainPrefab, stairPrefab;
    int numTiles = 3;
    bool left = false;
    System.Random random = new System.Random();

    // Start is called before the first frame update
    void Start() {
        GameObject obj;
        TerrainType type = TerrainType.Plain;
        Vector3 pos = new Vector3(0, 0, 0);
        for (uint i = 0; i < 15; ++i) {
            obj = (GameObject)Instantiate(buttonPrefab);
            obj.transform.Translate(pos);
            obj.transform.parent = transform;
            int size = random.Next(2, 10);
            if (left) ++pos.x;
            else ++pos.z;
            for (uint j = 0; j < size; ++j) {
                if (random.Next(100) < 20) type = TerrainType.Stair;
                else type = TerrainType.Plain;
                obj = (GameObject)Instantiate(GetPrefabTerrainType(type));
                obj.transform.Translate(pos);
                obj.transform.parent = transform;
                if (left) ++pos.x;
                else ++pos.z;
                if (type == TerrainType.Stair) --pos.y;
            }
            left = !left;
        }
    }

    GameObject GetPrefabTerrainType(TerrainType type) {
        switch(type) 
        {
            case TerrainType.Plain: return plainPrefab;
            case TerrainType.Button: return buttonPrefab;
            case TerrainType.Stair: return stairPrefab;
        }
        return null;
    }

    void Update() {
    }
}