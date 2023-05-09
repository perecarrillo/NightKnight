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
    Vector3 pos;
    // TODO: fer una cua per tenir els chunks i anar eliminant-los
    System.Random random = new System.Random();


    void GenerateTerrain() {
        GameObject obj;
        TerrainType type = TerrainType.Plain;
        GameObject chunk = new GameObject();
        chunk.transform.Translate(pos);
        for (uint i = 0; i < 5; ++i) {
            obj = (GameObject)Instantiate(buttonPrefab);
            obj.transform.Translate(pos);
            obj.transform.parent = chunk.transform;

            if (i == 3)
                transform.position = (pos + new Vector3(0, 1, 0));

            // section generation
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
                obj.transform.parent = chunk.transform;
                if (left) {
                    ++pos.x;
                }
                else ++pos.z;
                if (type == TerrainType.Stair) {
                    pos.y -= 0.5f;
                    if (!left) obj.transform.Rotate(new Vector3(0, -90, 0));
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
    // Start is called before the first frame update
    void Start() {
        pos = new Vector3(0, 0, 0);
        GenerateTerrain();
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

    void OnTriggerEnter(Collider other) {
        Debug.Log("Collision!");
        GenerateTerrain();
    }

    void Update() {
    }
}