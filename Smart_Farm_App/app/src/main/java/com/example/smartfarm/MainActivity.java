package com.example.smartfarm;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity {
    TextView btnTmp, btnHum, btnFan, btnHeat, btnLeftDoor, btnRightDoor;
    ImageView imageView;
    Button button;
    Switch control;
    public boolean stTemOn = false;
    public boolean stHumOn = false;

    Handler handler = new Handler();
    int targetTem, targetHum;
    boolean ConnectionState;
    public static Context context_main;
    public int curTem1, curTem2, curHum1, curHum2, leftWindow, rightWindow, fanState, heaterState,
            getCurTem1, getCurTem2, getCurHum1, getCurHum2, getLw, getRw, getFs, getHs;

    String urlCtrl = "http://192.168.0.38/setManualControl/";
    String urlTrd = "http://192.168.0.38/getState";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setTitle("SmartFarm");

        context_main = this;

        targetTem = 25;
        targetHum = 40;
        curTem1 = 0;
        curTem2 = 0;




        curHum1 = 0;
        curHum2 = 0;

        // 온도
        TextView btnTmp = findViewById(R.id.btnTmp);
        btnTmp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), SettingTemActivity.class);
                intent.putExtra("TargetTem", targetTem);
                intent.putExtra("CurrentTem1", curTem1);
                intent.putExtra("CurrentTem2", curTem2);
                startActivityForResult(intent,0);
            }
        });

        // 습도
        TextView btnHum = findViewById(R.id.btnHum);
        btnHum.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), SettingHumActivity.class);
                intent.putExtra("TargetHum", targetHum);
                intent.putExtra("CurrentHum1", curHum1);
                intent.putExtra("CurrentHum2", curHum2);
                startActivityForResult(intent,1);
            }
        });

        // 팬
        TextView btnFan = findViewById(R.id.btnFan);
        btnFan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), SettingFanActivity.class);
                startActivity(intent);
            }
        });

        // 열풍기
        TextView btnHeat = findViewById(R.id.btnHeat);
        btnHeat.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), SettingHeatActivity.class);
                startActivity(intent);
            }
        });

        // 좌측 개폐기
        TextView btnLeftDoor = findViewById(R.id.btnLeftDoor);
        btnLeftDoor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), SettingWindowActivity.class);
                startActivity(intent);
            }
        });

        // 우측 개폐기
        TextView btnRightDoor = findViewById(R.id.btnRightDoor);
        btnRightDoor.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), SettingWindowActivity.class);
                startActivity(intent);
            }
        });

        btnHum.setClickable(false);
        btnFan.setClickable(false);
        btnHeat.setClickable(false);
        btnLeftDoor.setClickable(false);
        btnRightDoor.setClickable(false);


        //스위치
        Switch control = findViewById(R.id.control);
        control.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                if (control.isChecked()==true) {
                    btnHum.setClickable(true);
                    btnFan.setClickable(true);
                    btnHeat.setClickable(true);
                    btnLeftDoor.setClickable(true);
                    btnRightDoor.setClickable(true);
                    String urlCtrlN = urlCtrl + "2";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            request(urlCtrlN);
                        }
                    }).start();
                } else {
                    btnHum.setClickable(false);
                    btnFan.setClickable(false);
                    btnHeat.setClickable(false);
                    btnLeftDoor.setClickable(false);
                    btnRightDoor.setClickable(false);
                    String urlCtrlN = urlCtrl + "1";
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            request(urlCtrlN);
                        }
                    }).start();
                }

            }
        });

        //현재 상태(온습도, 개폐기/팬/열풍기 가동 여부) 갱신용 Thread
        new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    try {
                        SystemClock.sleep(1000);
                        getState(urlTrd);
                        //서버로부터 받아온 값이 기존값과 다를 경우 세팅 : 온도
                        if(curTem1 != getCurTem1 || curTem2 != getCurTem2){
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    btnTmp.setText("현재 온도\n센서1 : " + curTem1 + "\n센서2 : " + curTem2 + "\n목표 온도 : " + targetTem);
                                    if(stTemOn == true){
                                        ((SettingTemActivity)SettingTemActivity.context_setTem).tvTem.
                                                setText("센서1 : "+ Integer.toString(curTem1) + "도\n센서2 : " + Integer.toString(curTem2) + "도\n");
                                    };
                                }
                            });
                            getCurTem1 = curTem1;
                            getCurTem2 = curTem2;
                        }

                        //서버로부터 받아온 값이 기존값과 다를 경우 세팅 : 습도
                        if(curHum1 != getCurHum1 || curHum2 != getCurHum2){
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    btnHum.setText("현재 습도\n센서1 : " + curHum1 + "\n센서2 : " + curHum2 + "\n목표 습도 : " + targetHum);
                                    if(stHumOn == true){
                                        ((SettingHumActivity)SettingHumActivity.context_setHum).tvHum.
                                                setText("센서1 : "+ Integer.toString(curHum1) + "도\n센서2 : " + Integer.toString(curHum2) + "도\n");
                                    };
                                }
                            });
                            getCurHum1 = curHum1;
                            getCurHum2 = curHum2;
                        }

                        //서버로부터 받아온 값이 기존값과 다를 경우 세팅 : 좌측개폐기
                        if(getLw != leftWindow){
                            switch (leftWindow){
                                case 0 :
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnLeftDoor.setText("좌측개폐기\n닫힘");
                                        }
                                    });
                                    break;

                                case 1 :
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnLeftDoor.setText("좌측개폐기\n열림");
                                        }
                                    });
                                    break;
                            }
                            getLw = leftWindow;
                        }

                        //서버로부터 받아온 값이 기존값과 다를 경우 세팅 : 우측개폐기
                        if(getRw != rightWindow){
                            switch (rightWindow){
                                case 0 :
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnRightDoor.setText("우측개폐기\n닫힘");
                                        }
                                    });
                                    break;

                                case 1 :
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnRightDoor.setText("우측개폐기\n열림");
                                        }
                                    });
                                    break;
                            }
                            getRw = rightWindow;
                        }

                        //서버로부터 받아온 값이 기존값과 다를 경우 세팅 : 팬
                        if(getFs != fanState){
                            switch (fanState){
                                case 0:
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnFan.setText("팬\nOff");
                                        }
                                    });
                                    break;
                                case 1:
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnFan.setText("팬\nOn");
                                        }
                                    });
                                    break;
                            }
                            getFs = fanState;
                        }
                        //서버로부터 받아온 값이 기존값과 다를 경우 세팅 : 열풍기
                        if(getHs != heaterState){
                            switch(heaterState){
                                case 0 :
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnHeat.setText("열풍기\nOff");
                                        }
                                    });
                                    break;
                                case 1:
                                    runOnUiThread(new Runnable() {
                                        @Override
                                        public void run() {
                                            btnHeat.setText("열풍기\nOn");
                                        }
                                    });
                                    break;
                            }
                            getHs = heaterState;
                        }
                        Thread.interrupted();
                    } catch (Exception e) {
                        e.printStackTrace();
                        Thread.interrupted();
                    }
                }
            }
        }).start();
    }

    //온습도 설정창 종료시 목표 온습도 값을 메인으로 받아옴
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode == 0){
            if(resultCode == RESULT_OK){
                targetTem = data.getIntExtra("TargetTemp", 0);
            }
        }

        if(requestCode == 1){
            if(resultCode == RESULT_OK){
                targetHum = data.getIntExtra("TargetHumi", 0);
            }
        }
    }




    // 서버 요청하기
    public void getState(String urlStr) {
        StringBuilder output = new StringBuilder();
        ConnectionState = false;
        try {
            URL url = new URL(urlStr);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            if (conn != null) {
                conn.setConnectTimeout(10000);
                conn.setRequestMethod("GET");
                conn.setDoInput(true);

                int resCode = conn.getResponseCode();

                BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                String line = null;

                while (true) {
                    line = reader.readLine();
                    if (line == null) {
                        break;
                    }
                    output.append(line + "\n");

                }
                //reader.close();

                conn.disconnect();
                reader.close();
                ConnectionState = true;
            }
        } catch (Exception ex) {
            Log.d("test" , "Error " + ex.getMessage());
            ConnectionState = false;
        }
        //센서에서 서버로 받아온 데이터를 앱에 표시
        if (ConnectionState == true) {
            String state = output.toString();
            String stateArray[] = state.split("/");
            Log.d("test", Integer.toString(targetTem));

            curTem1 = Integer.parseInt(stateArray[0]);
            curTem2 = Integer.parseInt(stateArray[1]);
            curHum1 = Integer.parseInt(stateArray[2]);
            curHum2 = Integer.parseInt(stateArray[3]);
            leftWindow  = Integer.parseInt(stateArray[4]);
            rightWindow  = Integer.parseInt(stateArray[5]);
            fanState  = Integer.parseInt(stateArray[6]);
            heaterState  = Integer.parseInt(stateArray[7]);
        }
    }

    // 서버에 입력값을 전달(목표 온습도, 팬, 열풍기 on/off 신호, 개폐기 열고닫기)
    public void request(String urlStr) {
        StringBuilder output = new StringBuilder();
        try {
            URL url = new URL(urlStr);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            if (conn != null) {
                conn.setConnectTimeout(10000);
                conn.setRequestMethod("GET");
                conn.setDoInput(true);

                int resCode = conn.getResponseCode();

                BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                String line = null;

                while (true) {
                    line = reader.readLine();
                    if (line == null) {
                        break;
                    }
                    output.append(line + "\n");
                }
                reader.close();
                conn.disconnect();
            }
        } catch (Exception ex) {
            Log.d("test" , "전달 실패.\nError: " + ex.getMessage());
        }
    }
}