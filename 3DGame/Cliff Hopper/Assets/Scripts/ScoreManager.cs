using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class ScoreManager : MonoBehaviour
{
    public static ScoreManager instance;

    public TMP_Text scoreText;
    public TMP_Text highScoreText;
    
    int score = 0;
    int highScore = 0;

    private void Awake() {
        instance = this;

    }
    // Start is called before the first frame update
    void Start()
    {
        highScore = PlayerPrefs.GetInt("highscore");
        scoreText.text = score.ToString();
        highScoreText.text = "TOP: " + highScore.ToString();
        
    }

    public void addPoints(int points) {
        score += points;
        if (score > highScore) {
            highScore = score;
            PlayerPrefs.SetInt("highscore", highScore);
        }
        scoreText.text = score.ToString();
        highScoreText.text = "TOP: " + highScore.ToString();
    }
}
