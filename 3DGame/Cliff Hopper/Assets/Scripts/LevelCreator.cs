using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum TerrainType {Button, Plain, Stair, Spike, Gap, Slowdown, Lava, Cannon};
public class LevelCreator : MonoBehaviour
{
    [SerializeField] GameObject player;
    public GameObject buttonPrefab, plainPrefab, stairPrefab, spikePrefab, gapPrefab, slowdownPrefab, lavaPrefab, cannonPrefab;
    public GameObject coinPrefab;
    bool noGap = true;
    bool left = false;
    Vector3 pos;
    Queue<GameObject> level = new Queue<GameObject>();
    System.Random random = new System.Random();


    void GenerateTerrain(bool start) {
        GameObject chunk = new GameObject();
        chunk.transform.Translate(pos);
        for (uint i = 0; i < 15; ++i) {
            GameObject obj;
            if (start) obj = (GameObject)Instantiate(plainPrefab);
            else obj = (GameObject)Instantiate(buttonPrefab);
            obj.transform.Translate(pos);
            obj.transform.parent = chunk.transform;
            obj.transform.Rotate(new Vector3(0, random.Next(4) * 90,0));

            if (i == 12)
                transform.position = (pos + new Vector3(0, 1, 0));

            // Cannon
            if (random.Next(100) < 100) {
                obj = (GameObject)Instantiate(cannonPrefab);
                Vector3 disp = new Vector3(2, 0, 0);
                if (left) disp = new Vector3(0, 0, 2);
                obj.transform.Translate(pos + disp);
                obj.transform.parent = chunk.transform;
                if (left) obj.transform.Rotate(new Vector3(0, 90, 0));
                else obj.transform.Rotate(new Vector3(0, 180, 0));
            }

            // section generation
            int size = random.Next(2, 10);
            if (start) size = 15;
            if (left) ++pos.x;
            else ++pos.z;
            noGap = true; // To not put a jump right after a turn
            for (uint j = 0; j < size; ++j) {
                if (random.Next(100) < 10) {
                    obj = (GameObject)Instantiate(coinPrefab);
                    obj.transform.Translate(pos + new Vector3(0, 1, 0));
                    obj.transform.parent = chunk.transform;
                }
                TerrainType type;
                int rand = random.Next(100);
                if ((!start || j > 6) && rand < 20) type = TerrainType.Stair;
                else if (rand < 30) {
                    type = TerrainType.Slowdown;
                }
                else if (!start && !noGap && rand < 30) {
                    type = TerrainType.Gap;
                    noGap = true;
                }
                else if (!start && !noGap && rand < 45) {
                    type = TerrainType.Spike;
                    noGap = true;
                }
                else if (!start && !noGap && rand < 50) {
                    type = TerrainType.Lava;
                    noGap = true;
                }
                else {
                    type = TerrainType.Plain;
                    noGap = false;
                }
                obj = (GameObject)Instantiate(GetPrefabTerrainType(type));
                if (type == TerrainType.Slowdown) {
                    obj.GetComponent<SlowDown>().player = player;
                }
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
            start = false;
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
        pos = new Vector3(0, 0, -4);
        GenerateTerrain(true);
    }

    GameObject GetPrefabTerrainType(TerrainType type) {
        switch(type) 
        {
            case TerrainType.Plain: return plainPrefab;
            case TerrainType.Button: return buttonPrefab;
            case TerrainType.Stair: return stairPrefab;
            case TerrainType.Spike: return spikePrefab;
            case TerrainType.Gap: return gapPrefab;
            case TerrainType.Slowdown: return slowdownPrefab;
            case TerrainType.Lava: return lavaPrefab;
            case TerrainType.Cannon: return cannonPrefab;
        }
        return null;
    }

    void OnTriggerEnter(Collider other) {
        Debug.Log("Generating new terrain");
        GenerateTerrain(false);
    }

    void Update() {
    }
}