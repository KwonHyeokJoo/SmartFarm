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

public class SettingTemActivity extends AppCompatActivity {
    boolean ConnectionState;
    public TextView tvTargetT, tvTem; // 온도 표시용 TextView
    Button btnReturn1, btnTmpDown, btnTmpUp, btnApply1; // 목표온도 조절용 Button
    NumberPicker npcTarTmp; // 목표온도 조절용 NumberPicker
    int CurrentTem1, CurrentTem2, targetTemp; // 현재 온도 1,2, 목표 온도
    public static Context context_setTem;
    String urlSetTem; // 목표 온도 전달용 url

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting_tem);

        context_setTem = this;

        ((MainActivity)MainActivity.context_main).stTemOn = true;

        Intent intent = getIntent();
//        targetTem = intent.getIntExtra("TargetTem", 30);    // 목표 온도
//        CurrentTem1 = intent.getIntExtra("CurrentTem1",30);   // 현재 온도1
//        CurrentTem2 = intent.getIntExtra("CurrentTem2",30);   // 현재 온도2
        CurrentTem1 = ((MainActivity)MainActivity.context_main).curTem1;
        CurrentTem2 = ((MainActivity)MainActivity.context_main).curTem2;
        targetTemp = ((MainActivity)MainActivity.context_main).targetTem;

        tvTem = (TextView) findViewById(R.id.tvTem);
        tvTem.setText("센서1 : "+ Integer.toString(CurrentTem1) + "도\n센서2 : " + Integer.toString(CurrentTem2) + "도\n");


        tvTargetT = (TextView) findViewById(R.id.tvTargetT);

        // 목표 온도값을 설정
        npcTarTmp = (NumberPicker) findViewById(R.id.npcTarTmp);
        npcTarTmp.setMinValue(0);   // 온도의 최소값 0도
        npcTarTmp.setMaxValue(50);  // 온도의 최댓값 50도
        npcTarTmp.setValue(targetTemp);
        npcTarTmp.setOnValueChangedListener(new NumberPicker.OnValueChangeListener() {
            @Override
            public void onValueChange(NumberPicker picker, int oldVal, int newVal) {
                targetTemp = newVal;
            }
        });

        // + 버튼을 누르면 값이 1씩 증가
        btnTmpUp = (Button) findViewById(R.id.btnTmpUp);
        btnTmpUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                npcTarTmp.setValue(npcTarTmp.getValue()+1);
            }
        });

        // - 버튼을 누르면 값이 1씩 감소
        btnTmpDown = (Button) findViewById(R.id.btnTmpDown);
        btnTmpDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                npcTarTmp.setValue(npcTarTmp.getValue()-1);
            }
        });

        //적용 버튼
        btnApply1 = (Button) findViewById(R.id.btnApply1);
        btnApply1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                targetTemp = npcTarTmp.getValue();
                urlSetTem = "http://192.168.0.38/setTargetTmp/" + Integer.toString(targetTemp);
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        ((MainActivity)MainActivity.context_main).request(urlSetTem);
                    }
                }).start();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        ((MainActivity)MainActivity.context_main).targetTem = targetTemp;
                    }
                });
            }
        });

        // 종료 버튼
        btnReturn1 = (Button) findViewById(R.id.btnReturn1);
        btnReturn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                targetTemp = npcTarTmp.getValue();
                Intent outIntent = new Intent(getApplicationContext(), MainActivity.class);
                outIntent.putExtra("TargetTemp", targetTemp);
                setResult(RESULT_OK, outIntent);
                finish();
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        ((MainActivity)MainActivity.context_main).stTemOn = false;
    }
}
