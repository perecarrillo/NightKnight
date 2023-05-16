using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum TerrainType {Button, Plain, Stair, Spike};
public class LevelCreator : MonoBehaviour
{
    public GameObject buttonPrefab, plainPrefab, stairPrefab, spikePrefab;
    bool noGap = true;
    bool left = false;
    Vector3 pos;
    Queue<GameObject> level = new Queue<GameObject>();
    // TODO: fer una cua per tenir els chunks i anar eliminant-los
    System.Random random = new System.Random();


    void GenerateTerrain() {
        GameObject chunk = new GameObject();
        chunk.transform.Translate(pos);
        for (uint i = 0; i < 15; ++i) {
            GameObject obj = (GameObject)Instantiate(buttonPrefab);
            obj.transform.Translate(pos);
            obj.transform.parent = chunk.transform;

            if (i == 12)
                transform.position = (pos + new Vector3(0, 1, 0));

            // section generation
            int size = random.Next(2, 10);
            if (left) ++pos.x;
            else ++pos.z;
            noGap = true; // To not put a jump right after a turn
            for (uint j = 0; j < size; ++j) {
                TerrainType type;
                int rand = random.Next(100);
                if (rand < 20) type = TerrainType.Stair;
                else if (!noGap && rand < 30) {
                    if (left) ++pos.x;
                    else ++pos.z;
                    noGap = true;
                    continue;
                }
                else if (!noGap && rand < 40) {
                    type = TerrainType.Spike;
                    noGap = true;
                }
                else {
                    type = TerrainType.Plain;
                    noGap = false;
                }
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
            }
            left = !left;
        }

        if (level.Count >= 3) {
            //Debug.Log("Deleting");
            GameObject prev = level.Dequeue();
            Destroy(prev);
        }
        level.Enqueue(chunk);
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