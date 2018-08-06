package com.example.administrator.ffmpegandroiddemo;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private String[] permissions = {android.Manifest.permission.READ_EXTERNAL_STORAGE, android.Manifest.permission
            .WRITE_EXTERNAL_STORAGE,
            android.Manifest.permission.CAMERA, android.Manifest.permission.RECORD_AUDIO, android.Manifest.permission
            .INTERNET, android.Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS};

    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        //全屏
//        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
//        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
//                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        setContentView(R.layout.activity_main);
//        ActivityCompat.requestPermissions(this, permissions, 0);
        TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());


        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface_view);
        SurfaceHolder holder = surfaceView.getHolder();
        final Surface surface = holder.getSurface();

        final String path = Environment.getExternalStorageDirectory()+File.separator+"DCIM/Camera/123.3gp";
        if (new File(path).exists()){


            holder.addCallback(new SurfaceHolder.Callback() {
                @Override public void surfaceCreated(SurfaceHolder holder)
                { //获取文件路径，这里将文件放置在手机根目录下

                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            playVideo(path, surface);
                        }
                    }).start();

                }

                @Override public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) { }

                @Override public void surfaceDestroyed(SurfaceHolder holder) { } });

        }else{
            Toast.makeText(this,"file not exist!!",Toast.LENGTH_SHORT).show();
        }

    }

    public native String stringFromJNI();
    public native int playVideo(String path ,Surface surfaceView);
}
