using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum TerrainType {Button, Plain, Stair, Spike};
public class LevelCreator : MonoBehaviour
{
    public GameObject buttonPrefab, plainPrefab, stairPrefab, spikePrefab;
    bool noGap = true;
    bool noSpike = true;
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
            noGap = true; // To not put a jump right after a turn
            noSpike = true;
            for (uint j = 0; j < size; ++j) {
                int rand = random.Next(100);
                if (rand < 20) type = TerrainType.Stair;
                else if (!noGap && rand < 30) {
                    if (left) ++pos.x;
                    else ++pos.z;
                    noGap = true;
                    continue;
                }
                else if (!noSpike && rand < 40) {
                    type = TerrainType.Spike;
                    if (left) ++pos.x;
                    else ++pos.z;
                    noSpike = true;
                    continue;
                }
                else type = TerrainType.Plain;
                obj = (GameObject)Instantiate(GetPrefabTerrainType(type));
                obj.transform.Translate(pos);
                obj.transform.parent = transform;
                if (left) {
                    ++pos.x;
                }
                else ++pos.z;
                if (type == TerrainType.Stair) {
                    pos.y -= 0.5f;
                    if (left) obj.transform.Rotate(new Vector3(0, 90, 0));
                }
                else {
                    obj.transform.Rotate(new Vector3(0, random.Next(4) * 90,0));
                }
                noGap = false;
                noSpike = false;
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
            case TerrainType.Spike: return spikePrefab;
        }
        return null;
    }

    void Update() {
    }
}