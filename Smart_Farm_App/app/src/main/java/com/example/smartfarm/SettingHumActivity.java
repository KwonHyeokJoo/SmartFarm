package com.example.smartfarm;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.NumberPicker;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class SettingHumActivity extends AppCompatActivity {

    boolean ConnectionState;
    public TextView tvTargetH, tvHum;// 습도 표시용 TextView
    Button btnReturn2, btnHumDown, btnHumUp, btnApply2; // 목표 습도 조절용 Button
    NumberPicker npcTarHum; // 목표 습도 조절용 NumberPicker
    int CurrentHum1, CurrentHum2, targetHum; // 현재 습도 1,2, 목표 습도
    public static Context context_setHum;
    String urlSetHum; // 목표 습도 전달용 url

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting_hum);

        context_setHum = this;

        Intent intent = getIntent();
        targetHum = intent.getIntExtra("TargetHum", 30);    // 목표 습도
        CurrentHum1 = intent.getIntExtra("CurrentHum1",30);   // 현재 습도1
        CurrentHum2 = intent.getIntExtra("CurrentHum2",30);   // 현재 습도2

        tvHum = (TextView) findViewById(R.id.tvHum);
        tvHum.setText("센서1 : " + Integer.toString(CurrentHum1) + "%\n센서2 : " + Integer.toString(CurrentHum2) + "%\n");


        tvTargetH = (TextView) findViewById(R.id.tvTargetH);

        // 목표 습도값을 설정
        npcTarHum = (NumberPicker) findViewById(R.id.npcTarHum);
        npcTarHum.setMinValue(0);       // 습도의 최솟값이 0
        npcTarHum.setMaxValue(100);      // 습도의 최댓값이 100
        npcTarHum.setValue(targetHum);
        /*npcTarHum.setOnValueChangedListener(new NumberPicker.OnValueChangeListener() {
            @Override
            public void onValueChange(NumberPicker picker, int oldVal, int newVal) {
                targetHum = newVal;
            }
        });*/

        // + 버튼을 누르면 값이 1씩 증가
        btnHumUp = (Button) findViewById(R.id.btnHumUp);
        btnHumUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                npcTarHum.setValue(npcTarHum.getValue()+1);
            }
        });

        // - 버튼을 누르면 값이 1씩 감소
        btnHumDown = (Button) findViewById(R.id.btnHumDown);
        btnHumDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                npcTarHum.setValue(npcTarHum.getValue()-1);
            }
        });

        // 적용버튼
        btnApply2 = (Button) findViewById(R.id.btnApply2);
        btnApply2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                targetHum = npcTarHum.getValue();
                urlSetHum = "http://192.168.0.38/setTargetHum/" + Integer.toString(targetHum);
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        ((MainActivity)MainActivity.context_main).request(urlSetHum);
                    }
                }).start();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        ((MainActivity)MainActivity.context_main).targetHum=targetHum;
                    }
                });
            }
        });


        // 종료 버튼
        btnReturn2 = (Button) findViewById(R.id.btnReturn2);
        btnReturn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent outIntent = new Intent(getApplicationContext(), MainActivity.class);
                targetHum = npcTarHum.getValue();
                outIntent.putExtra("TargetHumi", targetHum);
                setResult(RESULT_OK, outIntent);
                finish();
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        ((MainActivity)MainActivity.context_main).stHumOn = false;
    }
}