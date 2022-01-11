package com.example.smartfarm;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class SettingWindowActivity extends AppCompatActivity {

    String urlSetLeftWindow = "http://192.168.0.38/setLeftWindowC/";
    String urlSetRightWindow = "http://192.168.0.38/setRightWindowC/";

    @Override
    protected void onCreate( Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setting_window);

        Switch lwSwitch, rwSwitch;
        Button btnReturn5;

        //좌측개폐기 스위치
        lwSwitch = (Switch)findViewById(R.id.lwSwitch);
        lwSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (lwSwitch.isChecked()==true) {
                    String urlSetLeftWindow1 = urlSetLeftWindow + "1";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ((MainActivity)MainActivity.context_main).request(urlSetLeftWindow1);
                        }
                    }).start();
                } else {
                    String urlSetLeftWindow2 = urlSetLeftWindow + "2";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ((MainActivity)MainActivity.context_main).request(urlSetLeftWindow2);
                        }
                    }).start();
                }
            }
        });

        //우측개폐기 스위치
        rwSwitch = (Switch)findViewById(R.id.rwSwitch);
        rwSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if (rwSwitch.isChecked()==true) {
                    String urlSetRightWindow1 = urlSetRightWindow + "1";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ((MainActivity)MainActivity.context_main).request(urlSetRightWindow1);
                        }
                    }).start();
                } else {
                    String urlSetRightWindow2 = urlSetRightWindow + "2";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            ((MainActivity)MainActivity.context_main).request(urlSetRightWindow2);
                        }
                    }).start();
                }
            }
        });

        //종료
        btnReturn5 = (Button) findViewById(R.id.btnReturn5);
        btnReturn5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent outIntent = new Intent(getApplicationContext(), MainActivity.class);
                finish();
            }
        });
    }
}
