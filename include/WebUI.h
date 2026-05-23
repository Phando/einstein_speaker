#ifndef WEBUI_H
#define WEBUI_H

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>Einstein Remote</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,sans-serif;background:#1a1a2e;color:#fff;min-height:100vh;padding:16px;user-select:none;-webkit-user-select:none;touch-action:manipulation}
.header{display:flex;align-items:center;justify-content:space-between;padding:8px 4px;margin-bottom:12px}
.header h1{flex:1;text-align:center;font-size:1.3em}
.dot{width:10px;height:10px;border-radius:50%;background:#f44;margin-right:8px}
.dot.ok{background:#4f4}
.section{margin:12px 0}
.section-label{font-size:0.75em;text-transform:uppercase;letter-spacing:1px;opacity:0.5;margin:4px 0 6px 4px}
.row{display:grid;gap:10px}
.row2{grid-template-columns:1fr 1fr}
.row3{grid-template-columns:1fr 1fr 1fr}
.btn{border:none;border-radius:12px;font-size:1em;font-weight:600;padding:20px 8px;cursor:pointer;transition:transform 0.05s,filter 0.05s;text-align:center;min-height:60px;display:flex;align-items:center;justify-content:center}
.btn:active,.btn.active{transform:scale(0.95);filter:brightness(0.7)}
.btn-dark{background:#2a2a3e;color:#aaa;border:2px solid #3a3a4e}
.btn-dark.lit{background:#444;color:#fff;border-color:#888;box-shadow:0 0 8px rgba(255,255,255,0.2)}
.btn-white{background:#f0f0f0;color:#222}
.btn-blue{background:#0176D3;color:#fff}
.btn-coral{background:#FF6B5B;color:#fff}
.btn-amber{background:#FFAA00;color:#222}
.btn-lime{background:#A8E000;color:#222}
.btn-cyan{background:#00E5CC;color:#222}
.btn-turquoise{background:#5BC5D4;color:#222}
.btn-purple{background:#7B2FBE;color:#fff}
.btn-magenta{background:#D4007A;color:#fff}
.btn-pink{background:#FF69B4;color:#222}
.btn-lavender{background:#B39DDB;color:#222}
.btn-crazy{background:linear-gradient(135deg,#f44,#ff0,#0f0,#0ff,#f0f,#f44);background-size:300% 300%;animation:rainbow 3s ease infinite;color:#fff;text-shadow:0 1px 3px rgba(0,0,0,0.5)}
@keyframes rainbow{0%{background-position:0% 50%}50%{background-position:100% 50%}100%{background-position:0% 50%}}
.settings{background:#16213e;border-radius:12px;margin:16px 0;border:none}
.settings-toggle{padding:14px 16px;font-size:1em;font-weight:600;cursor:pointer;list-style:none;opacity:0.7}
.settings-toggle::-webkit-details-marker{display:none}
.settings-toggle::after{content:' \25BC';font-size:0.7em}
.settings[open] .settings-toggle::after{content:' \25B2'}
.settings-body{padding:0 16px 16px}
.settings label{display:block;font-size:0.85em;margin:8px 0 4px;opacity:0.8}
.settings input{width:100%;padding:10px;border-radius:8px;border:1px solid #444;background:#0f3460;color:#fff;font-size:1em}
.settings .btn-row{display:flex;gap:8px;margin-top:12px}
.settings .btn-save{flex:1;background:#388e3c;color:#fff;border:none;border-radius:8px;padding:12px;font-size:1em;font-weight:600;cursor:pointer}
.settings .btn-forget{flex:1;background:#c62828;color:#fff;border:none;border-radius:8px;padding:12px;font-size:1em;font-weight:600;cursor:pointer}
.status{font-size:0.8em;opacity:0.6;margin-top:8px;text-align:center}
</style>
</head>
<body>
<div class="header">
  <div class="dot" id="dot"></div>
  <h1>Einstein Remote</h1>
</div>

<div class="section">
  <div class="section-label">Einstein</div>
  <div class="row row2">
    <button class="btn btn-white" data-action="white_listen" data-mode="hold">Listen</button>
    <button class="btn btn-white" data-action="white_speak" data-mode="hold">Speak</button>
  </div>
</div>

<div class="section">
  <div class="section-label">Alexa</div>
  <div class="row row2">
    <button class="btn btn-blue" data-action="color_listen" data-mode="hold">Listen</button>
    <button class="btn btn-blue" data-action="color_speak" data-mode="hold">Speak</button>
  </div>
</div>

<div class="section">
  <div class="section-label">Speak with Color</div>
  <div class="row row3">
    <button class="btn btn-coral" data-action="speak_coral" data-mode="hold">&nbsp;</button>
    <button class="btn btn-amber" data-action="speak_amber" data-mode="hold">&nbsp;</button>
    <button class="btn btn-lime" data-action="speak_lime" data-mode="hold">&nbsp;</button>
  </div>
  <div class="row row3" style="margin-top:10px">
    <button class="btn btn-cyan" data-action="speak_cyan" data-mode="hold">&nbsp;</button>
    <button class="btn btn-turquoise" data-action="speak_turquoise" data-mode="hold">&nbsp;</button>
    <button class="btn btn-purple" data-action="speak_purple" data-mode="hold">&nbsp;</button>
  </div>
  <div class="row row3" style="margin-top:10px">
    <button class="btn btn-magenta" data-action="speak_magenta" data-mode="hold">&nbsp;</button>
    <button class="btn btn-pink" data-action="speak_pink" data-mode="hold">&nbsp;</button>
    <button class="btn btn-lavender" data-action="speak_lavender" data-mode="hold">&nbsp;</button>
  </div>
  <div class="row" style="margin-top:10px">
    <button class="btn btn-crazy" data-action="crazy" data-mode="hold">&nbsp;</button>
  </div>
</div>

<div class="section">
  <div class="section-label">Microphone</div>
  <div class="row row2">
    <button class="btn btn-dark" data-action="mic_on" data-mode="radio" data-group="mic">Mic On</button>
    <button class="btn btn-dark" data-action="mic_off" data-mode="radio" data-group="mic">Mic Off</button>
  </div>
</div>

<div class="section">
  <div class="row">
    <button class="btn btn-dark" data-action="all_off" data-mode="radio" data-group="power">ALL OFF</button>
  </div>
</div>

<details class="settings">
  <summary class="settings-toggle">Firmware Update</summary>
  <div class="settings-body">
    <label for="firmware">Select .bin file</label>
    <input type="file" id="firmware" accept=".bin">
    <div class="btn-row">
      <button class="btn-save" id="uploadBtn">Upload &amp; Reboot</button>
    </div>
    <div class="status" id="otaStatus"></div>
  </div>
</details>

<details class="settings" id="settings">
  <summary class="settings-toggle">WiFi Setup</summary>
  <div class="settings-body">
    <label for="ssid">Network Name (SSID)</label>
    <input type="text" id="ssid" autocomplete="off" autocapitalize="off">
    <label for="pass">Password</label>
    <input type="password" id="pass">
    <div class="btn-row">
      <button class="btn-save" id="saveBtn">Save &amp; Reboot</button>
      <button class="btn-forget" id="forgetBtn">Forget WiFi</button>
    </div>
    <div class="status" id="wifiStatus"></div>
  </div>
</details>

<script>
var ws;

function connect() {
  ws = new WebSocket('ws://' + location.host + '/ws');
  ws.onopen = function() {
    document.getElementById('dot').classList.add('ok');
  };
  ws.onclose = function() {
    document.getElementById('dot').classList.remove('ok');
    setTimeout(connect, 1000);
  };
  ws.onmessage = function(e) {
    var msg = e.data;
    if (msg.indexOf('STATUS:') === 0) {
      var parts = msg.split(':');
      var el = document.getElementById('wifiStatus');
      if (parts[1] === 'sta') {
        el.textContent = 'Connected to "' + parts[2] + '" @ ' + parts[3];
      } else {
        el.textContent = 'AP Mode - not connected to WiFi';
      }
    }
  };
}
connect();

function clearRadioGroup(group) {
  document.querySelectorAll('[data-group="' + group + '"]').forEach(function(b) {
    b.classList.remove('lit');
  });
}

document.querySelectorAll('[data-action]').forEach(function(btn) {
  var action = btn.dataset.action;
  var mode = btn.dataset.mode;

  if (mode === 'hold') {
    btn.addEventListener('touchstart', function(e) {
      e.preventDefault();
      btn.classList.add('active');
      send('ON:' + action);
    });
    btn.addEventListener('touchend', function(e) {
      e.preventDefault();
      btn.classList.remove('active');
      send('OFF');
    });
    btn.addEventListener('touchcancel', function() {
      btn.classList.remove('active');
      send('OFF');
    });
    btn.addEventListener('mousedown', function() {
      btn.classList.add('active');
      send('ON:' + action);
    });
    btn.addEventListener('mouseup', function() {
      btn.classList.remove('active');
      send('OFF');
    });
    btn.addEventListener('mouseleave', function() {
      if (btn.classList.contains('active')) {
        btn.classList.remove('active');
        send('OFF');
      }
    });
  }
  else if (mode === 'radio') {
    function handleRadio(e) {
      if (e) e.preventDefault();
      var group = btn.dataset.group;
      var wasLit = btn.classList.contains('lit');
      clearRadioGroup(group);
      if (!wasLit) {
        btn.classList.add('lit');
        send('ON:' + action);
      } else {
        send('OFF');
      }
      if (action === 'all_off') {
        clearRadioGroup('mic');
      }
    }
    btn.addEventListener('touchstart', handleRadio);
    btn.addEventListener('mousedown', handleRadio);
  }
});

function send(msg) {
  if (ws && ws.readyState === 1) ws.send(msg);
}

document.getElementById('saveBtn').addEventListener('click', function() {
  var ssid = document.getElementById('ssid').value;
  var pass = document.getElementById('pass').value;
  if (!ssid) return;
  send('WIFI:' + ssid + ':' + pass);
  document.getElementById('wifiStatus').textContent = 'Saving... rebooting...';
});

document.getElementById('forgetBtn').addEventListener('click', function() {
  if (confirm('Forget WiFi and reboot into AP mode?')) {
    send('FORGET');
    document.getElementById('wifiStatus').textContent = 'Forgetting... rebooting...';
  }
});

document.getElementById('uploadBtn').addEventListener('click', function() {
  var fileInput = document.getElementById('firmware');
  var status = document.getElementById('otaStatus');
  if (!fileInput.files.length) { status.textContent = 'No file selected.'; return; }
  var file = fileInput.files[0];
  var formData = new FormData();
  formData.append('update', file);
  status.textContent = 'Uploading... 0%';
  var xhr = new XMLHttpRequest();
  xhr.open('POST', '/update');
  xhr.upload.onprogress = function(e) {
    if (e.lengthComputable) {
      var pct = Math.round((e.loaded / e.total) * 100);
      status.textContent = 'Uploading... ' + pct + '%';
    }
  };
  xhr.onload = function() {
    if (xhr.responseText === 'OK') {
      status.textContent = 'Success! Rebooting...';
    } else {
      status.textContent = 'Update failed.';
    }
  };
  xhr.onerror = function() { status.textContent = 'Upload error.'; };
  xhr.send(formData);
});
</script>
</body>
</html>)rawliteral";

#endif
