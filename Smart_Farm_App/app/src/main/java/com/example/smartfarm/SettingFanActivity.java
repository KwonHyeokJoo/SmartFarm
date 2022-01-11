package com.example.smartfarm;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

public class SettingFanActivity extends AppCompatActivity {

    String urlSetFan = "http://192.168.0.38/setFanC/";
    String urlSetFanSpeed = "http://192.168.0.38/setFanSpeed/";
    int fanSpeed;

    @RequiresApi(api = Build.VERSION_CODES.O)
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting_fan);

        // 팬 속도 제어 및 속도 표시
        final TextView tvTargetF = (TextView) findViewById(R.id.tvTargetF);
        SeekBar SBfanSpeed = (SeekBar) findViewById(R.id.SBfanSpeed);
        Button btnReturn3;
        SBfanSpeed.setMax(255);

        SBfanSpeed.setEnabled(false);

        //팬 On/Off 여부 스위치
        Switch fanSwitch = findViewById(R.id.fanSwitch);
        fanSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                if (fanSwitch.isChecked()==true) {
                    SBfanSpeed.setEnabled(true);
                    String urlSetFan1 =  urlSetFan + "1";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ((MainActivity)MainActivity.context_main).request(urlSetFan1);
                        }
                    }).start();
                } else {
                    SBfanSpeed.setEnabled(false);
                    String urlSetFan2 =   urlSetFan + "2";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ((MainActivity)MainActivity.context_main).request(urlSetFan2);
                        }
                    }).start();
                }
            }
        });

        //팬 속도 조절용 SeekBar
        SBfanSpeed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                tvTargetF.setText("팬 속도 : " + (i * 23.5) + "rpm");
                fanSpeed = i;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                String urlSetFanSpeedN = urlSetFanSpeed + Integer.toString(fanSpeed);
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        ((MainActivity)MainActivity.context_main).request(urlSetFanSpeedN);
                    }
                }).start();
            }
        });

        //종료
        btnReturn3 = (Button) findViewById(R.id.btnReturn3);
        btnReturn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent outIntent = new Intent(getApplicationContext(), MainActivity.class);
                finish();
            }
        });
    }
}
