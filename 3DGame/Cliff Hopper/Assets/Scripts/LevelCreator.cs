using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum TerrainType {Plain, Button, Stair};
public class LevelCreator : MonoBehaviour
{
    public GameObject plainPrefab, buttonPrefab, stairPrefab;
    int numTiles = 3;
    System.Random random = new System.Random();

    // Start is called before the first frame update
    void Start() {
        GameObject obj;
        TerrainType type = TerrainType.Plain;
        int prevX = 0, prevY = 0, prevZ = 0;
        int x = 0, y = 0, z = 0;
        for (uint i = 0; i < 15; ++i) {
            obj = (GameObject)Instantiate(GetPrefabTerrainType(type));
            obj.transform.Translate(x, y, z);
            obj.transform.parent = transform;
            type = (TerrainType)(random.Next(numTiles));
            if (type == TerrainType.Button) {
                int rand = random.Next(2);
                if (rand == 0) {
                    ++x;
                }
                else if (rand == 1) {
                    ++z;
                }
            }
            prevX = x;
            prevY = y;
            prevZ = z;
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