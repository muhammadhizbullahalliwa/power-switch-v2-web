// ========== CONFIGURASI TINGKAT LANJUT ==========
#define BLYNK_TEMPLATE_ID "TMPL6mhCLY2HX"
#define BLYNK_TEMPLATE_NAME "No One"
#define BLYNK_AUTH_TOKEN "9V0U-QtNPYRO63Q-oR26CalZ1CB7fErt"

// ========== LIBRARY OPTIMIZED ==========
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <BlynkSimpleEsp32.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <WebServer.h>  // NEW: Web Server library
#include <ESPmDNS.h>    // NEW: For mDNS

// ========== PIN DEFINITIONS ==========
#define PIN_ZMPT1 34
#define PIN_ZMPT2 35
#define PIN_ACS 32
#define RELAY1_PIN 25
#define RELAY2_PIN 26
#define BUZZER_PIN 27
#define LED_STATUS 2

// ========== WIFI & TELEGRAM CONFIG ==========
char ssid[] = "KingFinix";
char pass[] = "";
#define BOT_TOKEN "8598852476:AAFsvYnX94UvAN6uSqgr1rFWP217YBGJMHg"
#define CHAT_ID "6293340373"

// ========== WEB SERVER CONFIG ==========
#define WEB_PORT 80
WebServer server(WEB_PORT);  // NEW: Web Server object

// ========== HTML PAGE ==========
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Power Switch System V2</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }
        
        body {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        
        header {
            text-align: center;
            margin-bottom: 30px;
            color: white;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        
        header h1 {
            font-size: 2.5rem;
            margin-bottom: 10px;
        }
        
        header p {
            font-size: 1.2rem;
            opacity: 0.9;
        }
        
        .developer-info {
            background: rgba(255,255,255,0.1);
            padding: 10px;
            border-radius: 10px;
            margin: 15px auto;
            max-width: 500px;
            backdrop-filter: blur(10px);
        }
        
        .developer-info a {
            color: #ffd700;
            text-decoration: none;
            margin: 0 10px;
            font-weight: bold;
        }
        
        .developer-info a:hover {
            text-decoration: underline;
        }
        
        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 25px;
            margin-bottom: 30px;
        }
        
        .card {
            background: white;
            border-radius: 15px;
            padding: 25px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
            transition: transform 0.3s ease;
        }
        
        .card:hover {
            transform: translateY(-5px);
        }
        
        .card-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
            padding-bottom: 15px;
            border-bottom: 2px solid #f0f0f0;
        }
        
        .card-header h2 {
            color: #4a5568;
            font-size: 1.5rem;
        }
        
        .card-header i {
            font-size: 2rem;
            color: #667eea;
        }
        
        .sensor-value {
            font-size: 2.5rem;
            font-weight: bold;
            color: #2d3748;
            text-align: center;
            margin: 15px 0;
        }
        
        .sensor-label {
            color: #718096;
            font-size: 1.1rem;
            text-align: center;
        }
        
        .status-badge {
            display: inline-block;
            padding: 8px 15px;
            border-radius: 20px;
            font-weight: bold;
            margin: 5px;
        }
        
        .status-online {
            background: #48bb78;
            color: white;
        }
        
        .status-offline {
            background: #f56565;
            color: white;
        }
        
        .status-warning {
            background: #ed8936;
            color: white;
        }
        
        .controls {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-top: 20px;
        }
        
        .btn {
            padding: 15px;
            border: none;
            border-radius: 10px;
            font-size: 1.1rem;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
        }
        
        .btn-primary {
            background: #4299e1;
            color: white;
        }
        
        .btn-primary:hover {
            background: #3182ce;
        }
        
        .btn-success {
            background: #48bb78;
            color: white;
        }
        
        .btn-success:hover {
            background: #38a169;
        }
        
        .btn-danger {
            background: #f56565;
            color: white;
        }
        
        .btn-danger:hover {
            background: #e53e3e;
        }
        
        .btn-warning {
            background: #ed8936;
            color: white;
        }
        
        .btn-warning:hover {
            background: #dd6b20;
        }
        
        .btn:disabled {
            opacity: 0.5;
            cursor: not-allowed;
        }
        
        .mode-toggle {
            display: flex;
            gap: 10px;
            margin: 15px 0;
        }
        
        .toggle-btn {
            flex: 1;
            padding: 12px;
            border: 2px solid #cbd5e0;
            background: white;
            border-radius: 8px;
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
        }
        
        .toggle-btn.active {
            background: #4299e1;
            color: white;
            border-color: #4299e1;
        }
        
        .log-section {
            margin-top: 30px;
        }
        
        #log {
            background: #f7fafc;
            border-radius: 10px;
            padding: 15px;
            height: 200px;
            overflow-y: auto;
            font-family: monospace;
            font-size: 0.9rem;
            border: 1px solid #e2e8f0;
        }
        
        .log-entry {
            padding: 5px 0;
            border-bottom: 1px solid #e2e8f0;
        }
        
        .log-time {
            color: #718096;
            margin-right: 10px;
        }
        
        .footer {
            text-align: center;
            margin-top: 40px;
            color: white;
            opacity: 0.8;
        }
        
        .refresh-btn {
            background: #9f7aea;
            color: white;
            border: none;
            padding: 12px 25px;
            border-radius: 8px;
            cursor: pointer;
            font-weight: bold;
            display: flex;
            align-items: center;
            gap: 8px;
            margin: 20px auto;
        }
        
        .refresh-btn:hover {
            background: #805ad5;
        }
        
        @media (max-width: 768px) {
            .dashboard {
                grid-template-columns: 1fr;
            }
            
            .controls {
                grid-template-columns: 1fr;
            }
            
            header h1 {
                font-size: 2rem;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1><i class="fas fa-bolt"></i> Power Switch System V2</h1>
            <p>Advanced Dual Source Power Management with Web Interface</p>
            
            <div class="developer-info">
                <p><i class="fas fa-code"></i> Developed by: mh_alliwa</p>
                <p>
                    <a href="https://t.me/mh_alliwa" target="_blank"><i class="fab fa-telegram"></i> Telegram</a>
                    <a href="https://instagram.com/mh.alliwa" target="_blank"><i class="fab fa-instagram"></i> Instagram</a>
                </p>
                <p><i class="fas fa-life-ring"></i> For help: Contact developer via Telegram</p>
            </div>
        </header>
        
        <div class="dashboard">
            <!-- Voltage Card -->
            <div class="card">
                <div class="card-header">
                    <h2><i class="fas fa-bolt"></i> Voltage Monitoring</h2>
                    <i class="fas fa-chart-line"></i>
                </div>
                <div class="sensor-value" id="voltage1">0.0 V</div>
                <div class="sensor-label">Source 1 Voltage</div>
                
                <div class="sensor-value" id="voltage2">0.0 V</div>
                <div class="sensor-label">Source 2 Voltage</div>
                
                <div class="status-badge" id="voltage-status">Checking...</div>
            </div>
            
            <!-- Current & Power Card -->
            <div class="card">
                <div class="card-header">
                    <h2><i class="fas fa-tachometer-alt"></i> Current & Power</h2>
                    <i class="fas fa-plug"></i>
                </div>
                <div class="sensor-value" id="current">0.000 A</div>
                <div class="sensor-label">Current</div>
                
                <div class="sensor-value" id="power">0.0 W</div>
                <div class="sensor-label">Power Consumption</div>
                
                <div class="sensor-value" id="energy">0.000 kWh</div>
                <div class="sensor-label">Total Energy</div>
            </div>
            
            <!-- System Status Card -->
            <div class="card">
                <div class="card-header">
                    <h2><i class="fas fa-cogs"></i> System Status</h2>
                    <i class="fas fa-server"></i>
                </div>
                
                <div class="sensor-label">System State</div>
                <div class="sensor-value" id="system-state">OFFLINE</div>
                
                <div class="sensor-label">Operation Mode</div>
                <div class="mode-toggle">
                    <button class="toggle-btn" id="mode-auto" onclick="setMode('auto')">AUTO</button>
                    <button class="toggle-btn" id="mode-manual" onclick="setMode('manual')">MANUAL</button>
                </div>
                
                <div class="sensor-label">Maintenance Mode</div>
                <div class="mode-toggle">
                    <button class="toggle-btn" id="mode-maintenance-off" onclick="setMaintenance(false)">OFF</button>
                    <button class="toggle-btn" id="mode-maintenance-on" onclick="setMaintenance(true)">ON</button>
                </div>
                
                <div class="sensor-label" style="margin-top: 20px;">WiFi Signal</div>
                <div class="sensor-value" id="wifi-rssi">0 dBm</div>
            </div>
        </div>
        
        <!-- Control Panel -->
        <div class="card">
            <div class="card-header">
                <h2><i class="fas fa-gamepad"></i> Manual Control Panel</h2>
                <i class="fas fa-sliders-h"></i>
            </div>
            
            <div class="controls">
                <button class="btn btn-success" onclick="controlSource(1)">
                    <i class="fas fa-power-off"></i> Activate Source 1
                </button>
                
                <button class="btn btn-success" onclick="controlSource(2)">
                    <i class="fas fa-power-off"></i> Activate Source 2
                </button>
                
                <button class="btn btn-danger" onclick="controlSource(0)">
                    <i class="fas fa-stop-circle"></i> Turn Off System
                </button>
                
                <button class="btn btn-warning" onclick="calibrateSystem()">
                    <i class="fas fa-tools"></i> Auto Calibrate
                </button>
            </div>
            
            <div class="controls" style="margin-top: 15px;">
                <button class="btn btn-primary" onclick="backupData()">
                    <i class="fas fa-download"></i> Backup Data
                </button>
                
                <button class="btn btn-primary" onclick="showCalibrationInfo()">
                    <i class="fas fa-info-circle"></i> Calibration Info
                </button>
                
                <button class="btn" onclick="restartSystem()" style="background: #805ad5; color: white;">
                    <i class="fas fa-redo"></i> Restart System
                </button>
            </div>
        </div>
        
        <!-- System Log -->
        <div class="card log-section">
            <div class="card-header">
                <h2><i class="fas fa-clipboard-list"></i> System Log</h2>
                <button class="refresh-btn" onclick="clearLog()">
                    <i class="fas fa-trash"></i> Clear Log
                </button>
            </div>
            <div id="log">
                <div class="log-entry"><span class="log-time">[00:00:00]</span> System initialized</div>
            </div>
        </div>
        
        <!-- Refresh Button -->
        <button class="refresh-btn" onclick="refreshData()">
            <i class="fas fa-sync-alt"></i> Refresh Data
        </button>
        
        <div class="footer">
            <p>Power Switch System V2 | Web Interface v1.0</p>
            <p>© 2024 Developed by mh_alliwa | Telegram: @mh_alliwa | Instagram: @mh.alliwa</p>
            <p>For technical assistance, contact developer via Telegram</p>
        </div>
    </div>
    
    <script>
        // Global variables
        let autoRefresh = true;
        let refreshInterval;
        
        // Format time function
        function formatTime(date) {
            return date.toTimeString().split(' ')[0];
        }
        
        // Add log entry
        function addLog(message) {
            const logDiv = document.getElementById('log');
            const time = new Date();
            const logEntry = document.createElement('div');
            logEntry.className = 'log-entry';
            logEntry.innerHTML = `<span class="log-time">[${formatTime(time)}]</span> ${message}`;
            logDiv.appendChild(logEntry);
            logDiv.scrollTop = logDiv.scrollHeight;
        }
        
        // Clear log
        function clearLog() {
            document.getElementById('log').innerHTML = '';
            addLog('Log cleared');
        }
        
        // Fetch data from ESP32
        async function fetchData() {
            try {
                const response = await fetch('/api/status');
                const data = await response.json();
                
                // Update voltage
                document.getElementById('voltage1').textContent = data.voltage1.toFixed(1) + ' V';
                document.getElementById('voltage2').textContent = data.voltage2.toFixed(1) + ' V';
                
                // Update current & power
                document.getElementById('current').textContent = data.current.toFixed(3) + ' A';
                document.getElementById('power').textContent = data.power.toFixed(1) + ' W';
                document.getElementById('energy').textContent = data.energy.toFixed(3) + ' kWh';
                
                // Update system state
                const stateElem = document.getElementById('system-state');
                stateElem.textContent = data.systemState;
                stateElem.className = 'sensor-value ' + 
                    (data.systemState.includes('ACTIVE') ? 'status-online' : 
                     data.systemState.includes('OFF') ? 'status-offline' : 'status-warning');
                
                // Update mode toggles
                document.getElementById('mode-auto').classList.toggle('active', data.autoMode);
                document.getElementById('mode-manual').classList.toggle('active', !data.autoMode);
                document.getElementById('mode-maintenance-on').classList.toggle('active', data.maintenanceMode);
                document.getElementById('mode-maintenance-off').classList.toggle('active', !data.maintenanceMode);
                
                // Update WiFi signal
                document.getElementById('wifi-rssi').textContent = data.wifiRSSI + ' dBm';
                
                // Update voltage status
                const voltageStatus = document.getElementById('voltage-status');
                if (data.voltage1 > 200 && data.voltage2 > 200) {
                    voltageStatus.textContent = 'NORMAL';
                    voltageStatus.className = 'status-badge status-online';
                } else if (data.voltage1 < 180 || data.voltage2 < 180) {
                    voltageStatus.textContent = 'LOW VOLTAGE';
                    voltageStatus.className = 'status-badge status-warning';
                } else {
                    voltageStatus.textContent = 'STABLE';
                    voltageStatus.className = 'status-badge status-online';
                }
                
                // Add log entry for significant changes
                if (data.systemState !== window.lastState) {
                    addLog(`System state changed to: ${data.systemState}`);
                    window.lastState = data.systemState;
                }
                
            } catch (error) {
                console.error('Error fetching data:', error);
                addLog('Error connecting to system');
            }
        }
        
        // Control power source
        async function controlSource(source) {
            try {
                const response = await fetch('/api/control', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({
                        action: source === 0 ? 'off' : 
                               source === 1 ? 'source1' : 'source2'
                    })
                });
                
                const result = await response.json();
                if (result.success) {
                    addLog(`Switched to ${source === 0 ? 'OFF' : 'Source ' + source}`);
                    fetchData();
                } else {
                    addLog(`Failed: ${result.message}`);
                }
            } catch (error) {
                addLog('Control failed: ' + error);
            }
        }
        
        // Set operation mode
        async function setMode(mode) {
            try {
                const response = await fetch('/api/mode', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({
                        mode: mode
                    })
                });
                
                const result = await response.json();
                if (result.success) {
                    addLog(`Mode changed to ${mode.toUpperCase()}`);
                    fetchData();
                }
            } catch (error) {
                addLog('Mode change failed');
            }
        }
        
        // Set maintenance mode
        async function setMaintenance(enable) {
            try {
                const response = await fetch('/api/maintenance', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: JSON.stringify({
                        enable: enable
                    })
                });
                
                const result = await response.json();
                if (result.success) {
                    addLog(`Maintenance mode ${enable ? 'ENABLED' : 'DISABLED'}`);
                    fetchData();
                }
            } catch (error) {
                addLog('Maintenance mode change failed');
            }
        }
        
        // Calibrate system
        async function calibrateSystem() {
            if (!confirm('WARNING: This will reset all calibration data. Ensure NO voltage is connected. Proceed?')) {
                return;
            }
            
            try {
                const response = await fetch('/api/calibrate', {
                    method: 'POST'
                });
                
                const result = await response.json();
                if (result.success) {
                    addLog('Calibration started. Please wait...');
                    setTimeout(() => {
                        addLog('Calibration complete!');
                        fetchData();
                    }, 15000);
                }
            } catch (error) {
                addLog('Calibration failed');
            }
        }
        
        // Backup data
        async function backupData() {
            try {
                const response = await fetch('/api/backup');
                const data = await response.json();
                
                // Create download link
                const blob = new Blob([JSON.stringify(data, null, 2)], {type: 'application/json'});
                const url = window.URL.createObjectURL(blob);
                const a = document.createElement('a');
                a.href = url;
                a.download = `power_system_backup_${new Date().toISOString().split('T')[0]}.json`;
                document.body.appendChild(a);
                a.click();
                document.body.removeChild(a);
                window.URL.revokeObjectURL(url);
                
                addLog('Backup downloaded successfully');
            } catch (error) {
                addLog('Backup failed');
            }
        }
        
        // Show calibration info
        async function showCalibrationInfo() {
            try {
                const response = await fetch('/api/calibration');
                const data = await response.json();
                
                const info = `
Calibration Information:
────────────────────
Zero Points:
• Source 1: ${data.zeroV1} (${(data.zeroV1 * 3.3 / 4095).toFixed(3)}V)
• Source 2: ${data.zeroV2} (${(data.zeroV2 * 3.3 / 4095).toFixed(3)}V)
• Current : ${data.zeroI} (${(data.zeroI * 3.3 / 4095).toFixed(3)}V)

Calibration Factors:
• Source 1: ${data.factorV1.toFixed(6)}
• Source 2: ${data.factorV2.toFixed(6)}
• Current : ${data.factorI.toFixed(6)}

Last Updated: ${data.lastCalibration}
`;
                
                alert(info);
                addLog('Calibration info displayed');
            } catch (error) {
                addLog('Failed to get calibration info');
            }
        }
        
        // Restart system
        async function restartSystem() {
            if (!confirm('Are you sure you want to restart the system?')) {
                return;
            }
            
            try {
                await fetch('/api/restart', {method: 'POST'});
                addLog('System restart initiated...');
                setTimeout(() => {
                    addLog('System should be restarting. Please wait 30 seconds.');
                }, 1000);
            } catch (error) {
                addLog('Restart command sent');
            }
        }
        
        // Refresh data
        function refreshData() {
            fetchData();
            addLog('Manual refresh');
        }
        
        // Toggle auto-refresh
        function toggleAutoRefresh() {
            autoRefresh = !autoRefresh;
            if (autoRefresh) {
                refreshInterval = setInterval(fetchData, 2000);
                document.getElementById('auto-refresh').textContent = 'Disable Auto Refresh';
                addLog('Auto refresh enabled');
            } else {
                clearInterval(refreshInterval);
                document.getElementById('auto-refresh').textContent = 'Enable Auto Refresh';
                addLog('Auto refresh disabled');
            }
        }
        
        // Initialize
        window.addEventListener('DOMContentLoaded', () => {
            addLog('Web interface loaded');
            fetchData();
            refreshInterval = setInterval(fetchData, 2000);
            
            // Add auto-refresh toggle button
            const refreshBtn = document.querySelector('.refresh-btn');
            refreshBtn.insertAdjacentHTML('afterend', 
                '<button class="refresh-btn" onclick="toggleAutoRefresh()" id="auto-refresh">Disable Auto Refresh</button>');
        });
    </script>
</body>
</html>
)rawliteral";

// ========== SENSOR CALIBRATION - FIXED VERSION ==========
typedef struct {
  float voltage1;
  float voltage2;
  float current;
  float power;
  float energy;
  uint32_t timestamp;
} SensorData;

typedef struct {
  int zeroV1;
  int zeroV2;
  int zeroI;
  float calibrationFactorV1;
  float calibrationFactorV2;
  float calibrationFactorI;
} CalibrationData;

// PERBAIKAN: Konstanta sensor yang benar
const float VREF = 3.3;              // ESP32 ADC reference voltage
const int ADC_MAX = 4095;            // 12-bit ADC resolution

// ZMPT101B: 250V AC input menghasilkan 1.8V AC output
const float ZMPT_SENSITIVITY = 0.0072; // 1.8V / 250V = 0.0072 V per Volt
const float ZMPT_OFFSET = 1.8 / 2;     // Zero point untuk ZMPT (tengah dari 0-1.8V)

// ACS712-20A: 100mV/A dengan Vcc/2 offset
const float ACS_SENSITIVITY = 0.100;  // 100mV per Ampere
const float ACS_VREF = 2.5;           // Voltage reference (Vcc/2)

// Thresholds - PERBAIKAN: Nilai yang lebih realistis
const float MIN_VOLTAGE = 0.01;      // Minimum voltage untuk dianggap valid
const float MIN_CURRENT = 0.01;      // Minimum current untuk dianggap valid
const float MAX_VOLTAGE = 500.0;     // Maksimum voltage sebelum shutdown
const float MAX_CURRENT = 25.0;      // Maksimum current sebelum shutdown

// PERBAIKAN: Deadband untuk menghilangkan noise
const float VOLTAGE_DEADBAND = 8.0;   // Noise threshold untuk voltage
const float CURRENT_DEADBAND = 0.02;  // Noise threshold untuk current

// System Parameters 
const float AUTO_SWITCH_VOLTAGE = 180.0;
const float AUTO_RESTORE_VOLTAGE = 190.0;
const uint32_t SWITCH_DELAY_NORMAL = 2000;
const uint32_t SWITCH_DELAY_AUTO = 5000;

// ========== SYSTEM OBJECTS ==========
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
BlynkTimer timer;
Preferences preferences;

// ========== SYSTEM VARIABLES ==========
SensorData sensors = {0};
CalibrationData calib = {2048, 2048, 2048, 1.0, 1.0, 1.0};

// System State
enum SystemState {
  STATE_OFF,
  STATE_SOURCE1_ACTIVE,
  STATE_SOURCE2_ACTIVE,
  STATE_SWITCHING,
  STATE_FAULT
};

SystemState systemState = STATE_OFF;
bool autoMode = false;
bool maintenanceMode = false;

// Switching Control
struct {
  SystemState targetState;
  uint32_t startTime;
  uint32_t delayTime;
  bool inProgress;
} switching = {STATE_OFF, 0, 0, false};

// Energy Management
float totalEnergy = 0;
uint32_t lastEnergyUpdate = 0;

// PERBAIKAN: Buffer filtering
#define FILTER_SIZE 15
float voltage1Buffer[FILTER_SIZE] = {0};
float voltage2Buffer[FILTER_SIZE] = {0};
float currentBuffer[FILTER_SIZE] = {0};
uint8_t bufferIndex = 0;

// Telegram
unsigned long lastTelegramUpdate = 0;
const unsigned long TELEGRAM_INTERVAL = 1000;

// Web Server
unsigned long lastWebUpdate = 0;
const unsigned long WEB_UPDATE_INTERVAL = 1000;

// ========== FUNCTION PROTOTYPES ==========
void initHardware();
void initWebServer();  // NEW: Web server initialization
void handleRoot();     // NEW: Handle root page
void handleAPI();      // NEW: Handle API requests
void handleControl();  // NEW: Handle control commands
void handleCalibrate(); // NEW: Handle calibration
void handleBackup();   // NEW: Handle backup
void handleRestart();  // NEW: Handle restart
void loadCalibration();
void saveCalibration();
void performAccurateCalibration();
void readSensorsImproved();
float readTrueRMS(uint8_t pin, int zeroPoint);
float convertToVoltage(float rmsADC, float calibFactor);
void manualCalibrationWithRealVoltage(int source, float realVoltage);
void recalibrateZeroPoint(int source);
void handleStateMachine();
void requestStateTransition(SystemState targetState, uint32_t delayMs);
void executeStateTransition();
void handleTelegramCommands();
void updateDisplay();
void sendTelegramAlert(String message);
void logEvent(String event);
void emergencyShutdown(String reason);
void toggleAutoMode(String chat_id);
void toggleMaintenanceMode(String chat_id);
String getMainMenuKeyboard();
String getCalibrationKeyboard();
String getSettingsKeyboard();
void testSensorAccuracy();
void updateBlynkData();
void checkSerialCommands();
void backupCalibration();
void handleManualCalibration(int source, String text, String chat_id);
String generateCalibrationInfo();
String generateTestReport();
String generateBackupData();
String getStateText();
String getCalibrationStatus();
String getDetailedCalibrationStatus();
String getLastCalibrationTime();
float calculateCalibrationError(int source, float realVoltage);
String getCalibrationQuality(float errorPercent);
String getAccuracyRecommendation(float v1, float v2);
float getStability(float value);
void resetAllCalibration();
void sendHelpMessage(String chat_id);
bool stringContains(String str, String substring);
void handleWebClients();  // NEW: Handle web clients

// ========== HELPER FUNCTION ==========
bool stringContains(String str, String substring) {
  return str.indexOf(substring) != -1;
}

// ========== WEB SERVER FUNCTIONS ==========
void initWebServer() {
  // Setup mDNS (for local access via hostname)
  if (!MDNS.begin("powerswitch")) {
    Serial.println("❌ Error setting up mDNS responder!");
  } else {
    Serial.println("✅ mDNS responder started: powerswitch.local");
  }
  
  // Route for root / web page
  server.on("/", HTTP_GET, handleRoot);
  
  // API routes
  server.on("/api/status", HTTP_GET, handleAPI);
  server.on("/api/control", HTTP_POST, handleControl);
  server.on("/api/mode", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      StaticJsonDocument<200> doc;
      deserializeJson(doc, server.arg("plain"));
      
      String mode = doc["mode"];
      if (mode == "auto") {
        autoMode = true;
      } else if (mode == "manual") {
        autoMode = false;
      }
      saveCalibration();
      
      StaticJsonDocument<100> response;
      response["success"] = true;
      response["message"] = "Mode updated";
      
      String jsonResponse;
      serializeJson(response, jsonResponse);
      server.send(200, "application/json", jsonResponse);
      
      logEvent("Mode changed via web: " + mode);
      sendTelegramAlert("🔧 Mode changed via Web: " + String(autoMode ? "AUTO" : "MANUAL"));
    }
  });
  
  server.on("/api/maintenance", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      StaticJsonDocument<200> doc;
      deserializeJson(doc, server.arg("plain"));
      
      bool enable = doc["enable"];
      maintenanceMode = enable;
      saveCalibration();
      
      if (maintenanceMode && systemState != STATE_OFF) {
        requestStateTransition(STATE_OFF, 1000);
      }
      
      StaticJsonDocument<100> response;
      response["success"] = true;
      response["message"] = "Maintenance mode updated";
      
      String jsonResponse;
      serializeJson(response, jsonResponse);
      server.send(200, "application/json", jsonResponse);
      
      logEvent("Maintenance mode changed via web: " + String(enable));
      sendTelegramAlert("🔧 Maintenance mode via Web: " + String(enable ? "ON" : "OFF"));
    }
  });
  
  server.on("/api/calibrate", HTTP_POST, handleCalibrate);
  server.on("/api/backup", HTTP_GET, handleBackup);
  server.on("/api/calibration", HTTP_GET, []() {
    StaticJsonDocument<500> doc;
    doc["zeroV1"] = calib.zeroV1;
    doc["zeroV2"] = calib.zeroV2;
    doc["zeroI"] = calib.zeroI;
    doc["factorV1"] = calib.calibrationFactorV1;
    doc["factorV2"] = calib.calibrationFactorV2;
    doc["factorI"] = calib.calibrationFactorI;
    doc["lastCalibration"] = getLastCalibrationTime();
    
    String jsonResponse;
    serializeJson(doc, jsonResponse);
    server.send(200, "application/json", jsonResponse);
  });
  
  server.on("/api/restart", HTTP_POST, handleRestart);
  
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });
  
  server.begin();
  Serial.println("✅ Web server started!");
  Serial.println("📱 Open browser and go to: http://" + WiFi.localIP().toString());
  Serial.println("   or: http://powerswitch.local");
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleAPI() {
  StaticJsonDocument<500> doc;
  
  // Sensor data
  doc["voltage1"] = sensors.voltage1;
  doc["voltage2"] = sensors.voltage2;
  doc["current"] = sensors.current;
  doc["power"] = sensors.power;
  doc["energy"] = totalEnergy;
  
  // System state
  doc["systemState"] = getStateText();
  doc["autoMode"] = autoMode;
  doc["maintenanceMode"] = maintenanceMode;
  doc["wifiRSSI"] = WiFi.RSSI();
  doc["ipAddress"] = WiFi.localIP().toString();
  doc["uptime"] = millis() / 1000;
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleControl() {
  if (server.hasArg("plain")) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    
    String action = doc["action"];
    bool success = false;
    String message = "";
    
    if (action == "off") {
      requestStateTransition(STATE_OFF, 1000);
      success = true;
      message = "System turning OFF";
    } 
    else if (action == "source1") {
      if (!maintenanceMode && !autoMode) {
        requestStateTransition(STATE_SOURCE1_ACTIVE, SWITCH_DELAY_NORMAL);
        success = true;
        message = "Switching to Source 1";
      } else {
        message = "Cannot switch - check mode settings";
      }
    }
    else if (action == "source2") {
      if (!maintenanceMode && !autoMode) {
        requestStateTransition(STATE_SOURCE2_ACTIVE, SWITCH_DELAY_NORMAL);
        success = true;
        message = "Switching to Source 2";
      } else {
        message = "Cannot switch - check mode settings";
      }
    }
    
    StaticJsonDocument<100> response;
    response["success"] = success;
    response["message"] = message;
    
    String jsonResponse;
    serializeJson(response, jsonResponse);
    server.send(200, "application/json", jsonResponse);
    
    if (success) {
      logEvent("Control via web: " + action);
      // PERBAIKAN: Menggunakan String concatenation yang benar
      String alertMsg = "🎮 Control via Web: ";
      action.toUpperCase(); // Ini tidak mengembalikan nilai, hanya mengubah string
      alertMsg += action;
      sendTelegramAlert(alertMsg);
    }
  }
}

void handleCalibrate() {
  StaticJsonDocument<100> response;
  
  if (maintenanceMode && systemState == STATE_OFF) {
    response["success"] = true;
    response["message"] = "Calibration started";
    
    String jsonResponse;
    serializeJson(response, jsonResponse);
    server.send(200, "application/json", jsonResponse);
    
    // Start calibration in background
    performAccurateCalibration();
    
    logEvent("Calibration started via web");
    sendTelegramAlert("🔧 Calibration started via Web");
  } else {
    response["success"] = false;
    response["message"] = "System must be OFF and in Maintenance mode";
    
    String jsonResponse;
    serializeJson(response, jsonResponse);
    server.send(400, "application/json", jsonResponse);
  }
}

void handleBackup() {
  StaticJsonDocument<500> doc;
  
  // Calibration data
  doc["zeroV1"] = calib.zeroV1;
  doc["zeroV2"] = calib.zeroV2;
  doc["zeroI"] = calib.zeroI;
  doc["factorV1"] = calib.calibrationFactorV1;
  doc["factorV2"] = calib.calibrationFactorV2;
  doc["factorI"] = calib.calibrationFactorI;
  
  // System settings
  doc["autoMode"] = autoMode;
  doc["maintenanceMode"] = maintenanceMode;
  doc["totalEnergy"] = totalEnergy;
  
  // Current readings
  doc["voltage1"] = sensors.voltage1;
  doc["voltage2"] = sensors.voltage2;
  doc["current"] = sensors.current;
  doc["power"] = sensors.power;
  
  // System info
  doc["systemState"] = getStateText();
  doc["ipAddress"] = WiFi.localIP().toString();
  doc["uptime"] = millis() / 1000;
  doc["timestamp"] = millis();
  doc["version"] = "Power Switch System V2 Web";
  
  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
  
  logEvent("Backup downloaded via web");
}

void handleRestart() {
  StaticJsonDocument<100> response;
  response["success"] = true;
  response["message"] = "System restarting...";
  
  String jsonResponse;
  serializeJson(response, jsonResponse);
  server.send(200, "application/json", jsonResponse);
  
  logEvent("Restart command via web");
  sendTelegramAlert("🔄 Restart initiated via Web");
  
  delay(1000);
  ESP.restart();
}

void handleWebClients() {
  server.handleClient();
  // PERBAIKAN: MDNS.update() tidak diperlukan di ESP32, hapus atau komentar
  // MDNS.update(); // Hanya untuk ESP8266
}

// ========== TELEGRAM KEYBOARD ==========
String getMainMenuKeyboard() {
  String keyboard = "[[";
  
  // Baris 1: Control Panel
  keyboard += "\"⚡ Sumber 1\",";
  keyboard += "\"⚡ Sumber 2\",";
  keyboard += "\"🚫 Matikan\"";
  
  keyboard += "],[";
  
  // Baris 2: Mode Control
  keyboard += "\"📊 Status System\",";
  if (autoMode) {
    keyboard += "\"🔄 AUTO ON\",";
  } else {
    keyboard += "\"🔧 MANUAL\",";
  }
  if (maintenanceMode) {
    keyboard += "\"🔧 MAINT ON\"";
  } else {
    keyboard += "\"🔧 MAINT OFF\"";
  }
  
  keyboard += "],[";
  
  // Baris 3: Calibration Menu
  keyboard += "\"🎯 Kalibrasi\",";
  keyboard += "\"⚙️ Settings\",";
  keyboard += "\"📈 Laporan\"";
  
  keyboard += "],[";
  
  // Baris 4: Advanced Features
  keyboard += "\"🔧 Test Sensor\",";
  keyboard += "\"💾 Backup Data\",";
  keyboard += "\"🆘 Bantuan\"";
  
  keyboard += "]]";
  
  return keyboard;
}

String getCalibrationKeyboard() {
  String keyboard = "[[";
  
  // Baris 1: Auto Calibration
  keyboard += "\"🎯 Auto Kalibrasi\",";
  keyboard += "\"🔄 Reset All Zero\"";
  
  keyboard += "],[";
  
  // Baris 2: Manual Calibration
  keyboard += "\"🔧 Kalibrasi V1\",";
  keyboard += "\"🔧 Kalibrasi V2\"";
  
  keyboard += "],[";
  
  // Baris 3: Zero Point Calibration
  keyboard += "\"🎯 Reset Zero V1\",";
  keyboard += "\"🎯 Reset Zero V2\",";
  keyboard += "\"🎯 Reset Zero I\"";
  
  keyboard += "],[";
  
  // Baris 4: Verification & Info
  keyboard += "\"📊 Cek Akurasi\",";
  keyboard += "\"💾 Simpan Data\",";
  keyboard += "\"🔍 Info Kalibrasi\"";
  
  keyboard += "],[";
  
  // Baris 5: Navigation
  keyboard += "\"🏠 Menu Utama\",";
  keyboard += "\"📈 Test Report\"";
  
  keyboard += "]]";
  
  return keyboard;
}

String getSettingsKeyboard() {
  String keyboard = "[[";
  
  keyboard += "\"📊 Parameters\",";
  keyboard += "\"🔍 Debug Info\",";
  keyboard += "\"⚙️ System Info\"";
  
  keyboard += "],[";
  
  keyboard += "\"🔄 Reset Energy\",";
  if (maintenanceMode) {
    keyboard += "\"🔧 MAINT ON ✅\",";
  } else {
    keyboard += "\"🔧 MAINT OFF\",";
  }
  if (autoMode) {
    keyboard += "\"🔄 AUTO ON\"";
  } else {
    keyboard += "\"🔧 MANUAL\"";
  }
  
  keyboard += "],[";
  
  keyboard += "\"🏠 Menu Utama\",";
  keyboard += "\"🔄 Restart System\"";
  
  keyboard += "]]";
  
  return keyboard;
}

// ========== HARDWARE INITIALIZATION ==========
void initHardware() {
  Serial.begin(115200);
  Serial.println("\n\n══════════════════════════════════");
  Serial.println("   POWER SWITCH SYSTEM V2");
  Serial.println("   WEB & TELEGRAM CALIBRATION SYSTEM");
  Serial.println("   Developer: mh_alliwa");
  Serial.println("   Telegram: @mh_alliwa");
  Serial.println("   Instagram: @mh.alliwa");
  Serial.println("══════════════════════════════════");
  
  // Konfigurasi ADC ESP32
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);  // 0-3.3V range
  
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  
  // Relay aktif LOW (safety first)
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_STATUS, LOW);
  
  // Initialize I2C and LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("POWER SWITCH V2");
  lcd.setCursor(0, 1);
  lcd.print("Web+Telegram Sys");
  
  delay(2000);
}

// ========== CALIBRATION FUNCTIONS ==========
void loadCalibration() {
  preferences.begin("power-switch", true);
  calib.zeroV1 = preferences.getInt("zeroV1", 2048);
  calib.zeroV2 = preferences.getInt("zeroV2", 2048);
  calib.zeroI = preferences.getInt("zeroI", 2048);
  calib.calibrationFactorV1 = preferences.getFloat("calV1", 1.0);
  calib.calibrationFactorV2 = preferences.getFloat("calV2", 1.0);
  calib.calibrationFactorI = preferences.getFloat("calI", 1.0);
  
  autoMode = preferences.getBool("autoMode", false);
  maintenanceMode = preferences.getBool("maintenanceMode", false);
  
  totalEnergy = preferences.getFloat("totalEnergy", 0);
  
  preferences.end();
  
  Serial.println("✅ System settings LOADED from flash");
  Serial.printf("Zero Points - V1:%d (%.3fV) V2:%d (%.3fV) I:%d (%.3fV)\n", 
                calib.zeroV1, calib.zeroV1 * VREF / ADC_MAX,
                calib.zeroV2, calib.zeroV2 * VREF / ADC_MAX,
                calib.zeroI, calib.zeroI * VREF / ADC_MAX);
  Serial.printf("Calibration Factors - V1:%.4f V2:%.4f I:%.4f\n", 
                calib.calibrationFactorV1, calib.calibrationFactorV2, calib.calibrationFactorI);
}

void saveCalibration() {
  preferences.begin("power-switch", false);
  
  // Save calibration data
  preferences.putInt("zeroV1", calib.zeroV1);
  preferences.putInt("zeroV2", calib.zeroV2);
  preferences.putInt("zeroI", calib.zeroI);
  preferences.putFloat("calV1", calib.calibrationFactorV1);
  preferences.putFloat("calV2", calib.calibrationFactorV2);
  preferences.putFloat("calI", calib.calibrationFactorI);
  
  // Save system settings
  preferences.putBool("autoMode", autoMode);
  preferences.putBool("maintenanceMode", maintenanceMode);
  preferences.putFloat("totalEnergy", totalEnergy);
  
  // Save timestamp
  preferences.putULong("lastCalTime", millis());
  
  preferences.end();
  
  Serial.println("💾 Settings saved to flash with timestamp");
}

void backupCalibration() {
  Serial.println("\n💾 CALIBRATION BACKUP (Copy these values):");
  Serial.println("══════════════════════════════════");
  Serial.printf("Zero V1: %d\n", calib.zeroV1);
  Serial.printf("Zero V2: %d\n", calib.zeroV2);
  Serial.printf("Zero I : %d\n", calib.zeroI);
  Serial.printf("Factor V1: %.6f\n", calib.calibrationFactorV1);
  Serial.printf("Factor V2: %.6f\n", calib.calibrationFactorV2);
  Serial.printf("Factor I : %.6f\n", calib.calibrationFactorI);
  Serial.println("══════════════════════════════════");
}

// ========== ACCURATE CALIBRATION ==========
void performAccurateCalibration() {
  Serial.println("\n🎯 STARTING ACCURATE CALIBRATION");
  Serial.println("⚠️  PLEASE ENSURE NO VOLTAGE IS CONNECTED!");
  
  lcd.clear();
  lcd.print("CALIBRATING...");
  lcd.setCursor(0, 1);
  lcd.print("NO VOLTAGE!");
  
  // Ensure all relays are OFF
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  delay(2000);
  
  // Collect zero point samples
  const int CALIB_SAMPLES = 10000;
  long sumV1 = 0, sumV2 = 0, sumI = 0;
  
  Serial.print("📊 Collecting zero point samples");
  
  for (int i = 0; i < CALIB_SAMPLES; i++) {
    sumV1 += analogRead(PIN_ZMPT1);
    sumV2 += analogRead(PIN_ZMPT2);
    sumI += analogRead(PIN_ACS);
    
    if (i % 2000 == 0) {
      Serial.print(".");
      lcd.setCursor(14, 0);
      lcd.print((i * 100) / CALIB_SAMPLES);
      lcd.print("%");
    }
    delayMicroseconds(50);
  }
  Serial.println();
  
  calib.zeroV1 = sumV1 / CALIB_SAMPLES;
  calib.zeroV2 = sumV2 / CALIB_SAMPLES;
  calib.zeroI = sumI / CALIB_SAMPLES;
  
  // Reset calibration factors to 1.0
  calib.calibrationFactorV1 = 1.0;
  calib.calibrationFactorV2 = 1.0;
  calib.calibrationFactorI = 1.0;
  
  saveCalibration();
  
  Serial.println("✅ CALIBRATION COMPLETE & SAVED");
  Serial.printf("Zero V1: %d (%.3fV)\n", calib.zeroV1, calib.zeroV1 * VREF / ADC_MAX);
  Serial.printf("Zero V2: %d (%.3fV)\n", calib.zeroV2, calib.zeroV2 * VREF / ADC_MAX);
  Serial.printf("Zero I : %d (%.3fV)\n", calib.zeroI, calib.zeroI * VREF / ADC_MAX);
  
  lcd.clear();
  lcd.print("CALIBRATION OK!");
  lcd.setCursor(0, 1);
  lcd.printf("SAVED TO FLASH");
  
  delay(3000);
}

float readTrueRMS(uint8_t pin, int zeroPoint) {
  const int samples = 2000;
  unsigned long sumSquares = 0;
  
  for (int i = 0; i < samples; i++) {
    int raw = analogRead(pin);
    int offset = raw - zeroPoint;
    sumSquares += (unsigned long)offset * offset;
    delayMicroseconds(40);
  }
  
  return sqrt((float)sumSquares / samples);
}

float convertToVoltage(float rmsADC, float calibFactor) {
  // Convert ADC reading to actual voltage
  float voltageADC = rmsADC * VREF / ADC_MAX;
  
  // ZMPT101B conversion formula:
  // Real Voltage = (ADC_Voltage × Calibration_Factor × 250) / (1.8 × 0.7071)
  // Where 1.8V is ZMPT output for 250V input
  // 0.7071 is RMS conversion factor (1/√2)
  
  float realVoltage = (voltageADC * calibFactor * 250.0) / (1.8 * 0.7071);
  
  return realVoltage;
}

// ========== MANUAL CALIBRATION WITH REAL VOLTAGE ==========
void manualCalibrationWithRealVoltage(int source, float realVoltage) {
  Serial.printf("\n🎯 MANUAL CALIBRATION SOURCE %d WITH %.1fV\n", source, realVoltage);
  
  if (realVoltage < 50 || realVoltage > 300) {
    Serial.println("❌ Voltage out of range (50-300V)");
    return;
  }
  
  lcd.clear();
  lcd.print("MANUAL CALIB");
  lcd.setCursor(0, 1);
  lcd.printf("Src%d %.0fV", source, realVoltage);
  
  delay(2000);
  
  // Read multiple samples for accuracy
  const int READINGS = 30;
  float sumMeasured = 0;
  
  Serial.print("📊 Reading samples");
  
  for (int i = 0; i < READINGS; i++) {
    float rms;
    if (source == 1) {
      rms = readTrueRMS(PIN_ZMPT1, calib.zeroV1);
    } else {
      rms = readTrueRMS(PIN_ZMPT2, calib.zeroV2);
    }
    
    float voltageADC = rms * VREF / ADC_MAX;
    float measuredVoltage = (voltageADC * 250.0) / (1.8 * 0.7071);
    
    sumMeasured += measuredVoltage;
    
    if (i % 5 == 0) {
      Serial.print(".");
      lcd.setCursor(14, 0);
      lcd.print((i * 100) / READINGS);
      lcd.print("%");
    }
    delay(100);
  }
  Serial.println();
  
  float avgMeasured = sumMeasured / READINGS;
  
  // Calculate new calibration factor
  float newCalibFactor = realVoltage / avgMeasured;
  
  if (source == 1) {
    float oldFactor = calib.calibrationFactorV1;
    calib.calibrationFactorV1 = newCalibFactor;
    Serial.printf("✅ V1 Cal Factor: %.4f -> %.4f (Change: %.2f%%)\n", 
                  oldFactor, newCalibFactor, 
                  (newCalibFactor - oldFactor) / oldFactor * 100);
  } else {
    float oldFactor = calib.calibrationFactorV2;
    calib.calibrationFactorV2 = newCalibFactor;
    Serial.printf("✅ V2 Cal Factor: %.4f -> %.4f (Change: %.2f%%)\n", 
                  oldFactor, newCalibFactor,
                  (newCalibFactor - oldFactor) / oldFactor * 100);
  }
  
  saveCalibration();
  
  // Verify calibration
  delay(1000);
  float verifyRMS = readTrueRMS((source == 1) ? PIN_ZMPT1 : PIN_ZMPT2, 
                               (source == 1) ? calib.zeroV1 : calib.zeroV2);
  float verifyADC = verifyRMS * VREF / ADC_MAX;
  float verifiedVoltage = convertToVoltage(verifyRMS, 
                                          (source == 1) ? calib.calibrationFactorV1 : calib.calibrationFactorV2);
  
  float errorPercent = abs(realVoltage - verifiedVoltage) / realVoltage * 100;
  
  Serial.printf("📊 Verification: Input %.1fV -> Measured %.1fV (Error: %.1f%%)\n",
                realVoltage, verifiedVoltage, errorPercent);
  
  lcd.clear();
  if (errorPercent < 5.0) {
    lcd.print("CALIB SUCCESS!");
  } else {
    lcd.print("CALIB WARNING!");
  }
  lcd.setCursor(0, 1);
  lcd.printf("%.0fV->%.0fV %.1f%%", realVoltage, verifiedVoltage, errorPercent);
  
  delay(3000);
}

void recalibrateZeroPoint(int source) {
  Serial.printf("\n🔄 RE-CALIBRATING ZERO POINT FOR SOURCE %d\n", source);
  
  lcd.clear();
  lcd.print("RE-ZERO CAL");
  lcd.setCursor(0, 1);
  lcd.printf("Source %d", source);
  
  delay(2000);
  
  long sum = 0;
  const int samples = 5000;
  
  Serial.print("📊 Collecting zero samples (ensure NO voltage)...");
  
  for (int i = 0; i < samples; i++) {
    if (source == 1) {
      sum += analogRead(PIN_ZMPT1);
    } else if (source == 2) {
      sum += analogRead(PIN_ZMPT2);
    } else if (source == 3) {
      sum += analogRead(PIN_ACS);
    }
    delayMicroseconds(100);
    
    if (i % 500 == 0) {
      Serial.print(".");
    }
  }
  Serial.println();
  
  int newZero = sum / samples;
  
  if (source == 1) {
    calib.zeroV1 = newZero;
  } else if (source == 2) {
    calib.zeroV2 = newZero;
  } else {
    calib.zeroI = newZero;
  }
  
  saveCalibration();
  
  Serial.printf("✅ New Zero Point: %d (%.3fV) - SAVED\n", newZero, newZero * VREF / ADC_MAX);
  
  lcd.clear();
  lcd.print("ZERO UPDATED!");
  lcd.setCursor(0, 1);
  lcd.printf("Saved to flash");
  
  delay(3000);
}

// ========== IMPROVED SENSOR READING ==========
void readSensorsImproved() {
  static unsigned long lastRead = 0;
  if (millis() - lastRead < 500) return;
  lastRead = millis();
  
  // Read RMS values
  float rmsV1 = readTrueRMS(PIN_ZMPT1, calib.zeroV1);
  float rmsV2 = readTrueRMS(PIN_ZMPT2, calib.zeroV2);
  float rmsI = readTrueRMS(PIN_ACS, calib.zeroI);
  
  // Convert to actual values
  float rawV1 = convertToVoltage(rmsV1, calib.calibrationFactorV1);
  float rawV2 = convertToVoltage(rmsV2, calib.calibrationFactorV2);
  
  // ACS712 conversion
  float voltageAdcI = rmsI * VREF / ADC_MAX;
  float rawI = (voltageAdcI - (VREF/2)) / ACS_SENSITIVITY * calib.calibrationFactorI;
  
  // Apply deadband for noise
  if (abs(rawV1) < VOLTAGE_DEADBAND) rawV1 = 0;
  if (abs(rawV2) < VOLTAGE_DEADBAND) rawV2 = 0;
  if (abs(rawI) < CURRENT_DEADBAND) rawI = 0;
  
  // Moving average filter
  voltage1Buffer[bufferIndex] = rawV1;
  voltage2Buffer[bufferIndex] = rawV2;
  currentBuffer[bufferIndex] = rawI;
  bufferIndex = (bufferIndex + 1) % FILTER_SIZE;
  
  // Calculate averages
  float sumV1 = 0, sumV2 = 0, sumI = 0;
  int countV1 = 0, countV2 = 0, countI = 0;
  
  for (int i = 0; i < FILTER_SIZE; i++) {
    if (voltage1Buffer[i] > MIN_VOLTAGE) {
      sumV1 += voltage1Buffer[i];
      countV1++;
    }
    if (voltage2Buffer[i] > MIN_VOLTAGE) {
      sumV2 += voltage2Buffer[i];
      countV2++;
    }
    if (abs(currentBuffer[i]) > MIN_CURRENT) {
      sumI += currentBuffer[i];
      countI++;
    }
  }
  
  // Only update if we have valid readings
  sensors.voltage1 = (countV1 > FILTER_SIZE/3) ? sumV1 / countV1 : 0;
  sensors.voltage2 = (countV2 > FILTER_SIZE/3) ? sumV2 / countV2 : 0;
  sensors.current = (countI > FILTER_SIZE/3) ? sumI / countI : 0;
  
  // Calculate power based on active source
  if (systemState == STATE_SOURCE1_ACTIVE && sensors.voltage1 > MIN_VOLTAGE) {
    sensors.power = sensors.voltage1 * abs(sensors.current);
  } else if (systemState == STATE_SOURCE2_ACTIVE && sensors.voltage2 > MIN_VOLTAGE) {
    sensors.power = sensors.voltage2 * abs(sensors.current);
  } else {
    sensors.power = 0;
  }
  
  // Update energy consumption
  if (sensors.power > 0 && lastEnergyUpdate > 0) {
    float hours = (millis() - lastEnergyUpdate) / 3600000.0; // Convert ms to hours
    totalEnergy += sensors.power * hours / 1000.0; // Convert W to kW
  }
  lastEnergyUpdate = millis();
  
  // Debug output
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 3000) {
    Serial.println("══════════════════════════════════");
    Serial.println("📊 SENSOR READINGS (IMPROVED)");
    Serial.println("══════════════════════════════════");
    Serial.printf("Voltage 1: %.1f V (Valid: %d/%d)\n", sensors.voltage1, countV1, FILTER_SIZE);
    Serial.printf("Voltage 2: %.1f V (Valid: %d/%d)\n", sensors.voltage2, countV2, FILTER_SIZE);
    Serial.printf("Current  : %.3f A (Valid: %d/%d)\n", sensors.current, countI, FILTER_SIZE);
    Serial.printf("Power    : %.1f W\n", sensors.power);
    Serial.printf("Energy   : %.3f kWh\n", totalEnergy);
    Serial.println("══════════════════════════════════");
    lastDebug = millis();
  }
}

// ========== STATE MACHINE ==========
void requestStateTransition(SystemState targetState, uint32_t delayMs) {
  if (switching.inProgress || systemState == STATE_FAULT) {
    return;
  }
  
  if (maintenanceMode && targetState != STATE_OFF) {
    sendTelegramAlert("⚠️ Maintenance mode aktif! Tidak bisa mengaktifkan sumber.");
    return;
  }
  
  switching.targetState = targetState;
  switching.startTime = millis();
  switching.delayTime = delayMs;
  switching.inProgress = true;
  systemState = STATE_SWITCHING;
  
  // Turn off all relays during switching
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  
  String msg;
  if (targetState == STATE_OFF) {
    msg = "⛔ *MEMATIKAN SYSTEM*";
    logEvent("System shutdown requested");
  } else if (targetState == STATE_SOURCE1_ACTIVE) {
    msg = "🔄 *SWITCH KE SUMBER 1*";
    logEvent("Switching to Source 1");
  } else {
    msg = "🔄 *SWITCH KE SUMBER 2*";
    logEvent("Switching to Source 2");
  }
  
  msg += "\n⏱️ Jeda keamanan: " + String(delayMs / 1000) + " detik";
  
  sendTelegramAlert(msg);
}

void executeStateTransition() {
  if (!switching.inProgress) return;
  
  if (millis() - switching.startTime < switching.delayTime) return;
  
  switching.inProgress = false;
  
  switch (switching.targetState) {
    case STATE_OFF:
      digitalWrite(RELAY1_PIN, HIGH);
      digitalWrite(RELAY2_PIN, HIGH);
      systemState = STATE_OFF;
      sendTelegramAlert("✅ *SYSTEM DIMATIKAN*\nSemua relay dalam keadaan OFF");
      logEvent("System turned OFF");
      break;
      
    case STATE_SOURCE1_ACTIVE:
      if (!maintenanceMode) {
        digitalWrite(RELAY1_PIN, LOW);
        digitalWrite(RELAY2_PIN, HIGH);
        systemState = STATE_SOURCE1_ACTIVE;
        sendTelegramAlert("✅ *SUMBER 1 AKTIF*\nTegangan: " + String(sensors.voltage1, 1) + "V");
        logEvent("Source 1 activated");
      }
      break;
      
    case STATE_SOURCE2_ACTIVE:
      if (!maintenanceMode) {
        digitalWrite(RELAY1_PIN, HIGH);
        digitalWrite(RELAY2_PIN, LOW);
        systemState = STATE_SOURCE2_ACTIVE;
        sendTelegramAlert("✅ *SUMBER 2 AKTIF*\nTegangan: " + String(sensors.voltage2, 1) + "V");
        logEvent("Source 2 activated");
      }
      break;
  }
  
  // Update Blynk
  Blynk.virtualWrite(V8, (systemState == STATE_SOURCE1_ACTIVE) ? 1 : 0);
  Blynk.virtualWrite(V9, (systemState == STATE_SOURCE2_ACTIVE) ? 1 : 0);
}

void handleStateMachine() {
  executeStateTransition();
  
  if (systemState == STATE_FAULT || maintenanceMode) {
    return;
  }
  
  // Auto mode logic
  if (autoMode && !switching.inProgress) {
    switch (systemState) {
      case STATE_SOURCE1_ACTIVE:
        if (sensors.voltage1 > 0 && sensors.voltage1 < AUTO_SWITCH_VOLTAGE && 
            sensors.voltage2 >= AUTO_SWITCH_VOLTAGE) {
          sendTelegramAlert("⚠️ *VOLTAGE DROP DETECTED*\nV1: " + String(sensors.voltage1, 1) + 
                           "V < " + String(AUTO_SWITCH_VOLTAGE) + "V\nAuto-switch ke Sumber 2...");
          requestStateTransition(STATE_SOURCE2_ACTIVE, SWITCH_DELAY_AUTO);
        }
        break;
        
      case STATE_SOURCE2_ACTIVE:
        if (sensors.voltage1 >= AUTO_RESTORE_VOLTAGE) {
          sendTelegramAlert("⚡ *VOLTAGE RESTORED*\nV1: " + String(sensors.voltage1, 1) + 
                           "V >= " + String(AUTO_RESTORE_VOLTAGE) + "V\nAuto-restore ke Sumber 1...");
          requestStateTransition(STATE_SOURCE1_ACTIVE, SWITCH_DELAY_AUTO);
        }
        break;
        
      case STATE_OFF:
        if (sensors.voltage1 >= AUTO_SWITCH_VOLTAGE) {
          sendTelegramAlert("🔛 *AUTO START*\nV1: " + String(sensors.voltage1, 1) + 
                           "V >= " + String(AUTO_SWITCH_VOLTAGE) + "V\nMengaktifkan Sumber 1...");
          requestStateTransition(STATE_SOURCE1_ACTIVE, SWITCH_DELAY_NORMAL);
        } else if (sensors.voltage2 >= AUTO_SWITCH_VOLTAGE) {
          sendTelegramAlert("🔛 *AUTO START*\nV2: " + String(sensors.voltage2, 1) + 
                           "V >= " + String(AUTO_SWITCH_VOLTAGE) + "V\nMengaktifkan Sumber 2...");
          requestStateTransition(STATE_SOURCE2_ACTIVE, SWITCH_DELAY_NORMAL);
        }
        break;
    }
  }
  
  // Safety checks
  if (sensors.voltage1 > MAX_VOLTAGE || sensors.voltage2 > MAX_VOLTAGE) {
    emergencyShutdown("Over-voltage protection");
  }
  
  if (abs(sensors.current) > MAX_CURRENT) {
    emergencyShutdown("Over-current protection");
  }
}

void emergencyShutdown(String reason) {
  Serial.println("🚨 EMERGENCY SHUTDOWN: " + reason);
  
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  systemState = STATE_FAULT;
  
  String message = "🚨 *EMERGENCY SHUTDOWN!*\n";
  message += "Reason: " + reason + "\n";
  message += "V1: " + String(sensors.voltage1, 1) + "V\n";
  message += "V2: " + String(sensors.voltage2, 1) + "V\n";
  message += "I: " + String(sensors.current, 2) + "A\n";
  message += "All relays turned OFF\n";
  message += "System requires manual reset";
  
  sendTelegramAlert(message);
  logEvent("EMERGENCY: " + reason);
  
  // Alert pattern
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
    digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
    delay(200);
  }
  
  digitalWrite(BUZZER_PIN, LOW);
}

// ========== TELEGRAM HANDLER COMPLETE ==========
void handleTelegramCommands() {
  if (millis() - lastTelegramUpdate < TELEGRAM_INTERVAL) return;
  
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "❌ Akses ditolak. ID Anda: " + chat_id, "");
      continue;
    }
    
    Serial.println("Telegram Command: " + text);
    
    // ========== MENU UTAMA ==========
    if (text == "/start" || text == "🏠 Menu Utama") {
      String welcome = "🤖 *SISTEM POWER SWITCH V2*\n";
      welcome += "━━━━━━━━━━━━━━━━━━\n";
      welcome += "*Status Saat Ini:*\n";
      welcome += "🔌 " + getStateText() + "\n";
      welcome += "⚡ V1: " + String(sensors.voltage1, 1) + "V | ";
      welcome += "V2: " + String(sensors.voltage2, 1) + "V\n";
      welcome += "🔋 I: " + String(sensors.current, 2) + "A | ";
      welcome += "P: " + String(sensors.power, 1) + "W\n";
      welcome += "🎛️ Mode: " + String(autoMode ? "AUTO" : "MANUAL") + " | ";
      welcome += "Maint: " + String(maintenanceMode ? "ON" : "OFF") + "\n";
      welcome += "━━━━━━━━━━━━━━━━━━\n";
      welcome += "*Kalibrasi:* " + getCalibrationStatus() + "\n";
      welcome += "━━━━━━━━━━━━━━━━━━\n";
      welcome += "*Web Interface:* http://" + WiFi.localIP().toString() + "\n";
      welcome += "*mDNS:* http://powerswitch.local\n";
      welcome += "📍 Developed by: mh_alliwa";
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, welcome, "Markdown", getMainMenuKeyboard(), true);
    }
    
    // ========== KALIBRASI MENU ==========
    else if (text == "🎯 Kalibrasi") {
      String calibMenu = "🔧 *MENU KALIBRASI*\n";
      calibMenu += "━━━━━━━━━━━━━━━━━━\n";
      calibMenu += "*Status Kalibrasi:*\n";
      calibMenu += getDetailedCalibrationStatus();
      calibMenu += "━━━━━━━━━━━━━━━━━━\n";
      calibMenu += "*Web Interface:* http://" + WiFi.localIP().toString() + "\n";
      calibMenu += "Pilih opsi di bawah:";
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, calibMenu, "Markdown", getCalibrationKeyboard(), true);
    }
    
    // ========== AUTO CALIBRATION ==========
    else if (text == "🎯 Auto Kalibrasi") {
      bot.sendMessage(CHAT_ID, "🔧 *MEMULAI KALIBRASI OTOMATIS*\n\n"
                    "⚠️ *PERINGATAN:*\n"
                    "1. Pastikan TIDAK ADA tegangan terhubung\n"
                    "2. Semua relay dalam keadaan OFF\n"
                    "3. Jangan sentuh sensor selama proses\n\n"
                    "Proses akan dimulai dalam 5 detik...", "Markdown");
      
      delay(5000);
      
      String progressMsg = "🔄 *Proses Kalibrasi*\n"
                          "━━━━━━━━━━━━━━━━━━\n"
                          "📊 Mengumpulkan sampel...\n"
                          "⏱️ Estimasi: 15 detik\n"
                          "🔌 Pastikan kondisi stabil";
      
      bot.sendMessage(CHAT_ID, progressMsg, "Markdown");
      
      performAccurateCalibration();
      
      String resultMsg = "✅ *KALIBRASI SELESAI!*\n"
                        "━━━━━━━━━━━━━━━━━━\n"
                        "📊 *Hasil Zero Point:*\n"
                        "• V1: " + String(calib.zeroV1) + " (" + String(calib.zeroV1 * VREF / ADC_MAX, 3) + "V)\n"
                        "• V2: " + String(calib.zeroV2) + " (" + String(calib.zeroV2 * VREF / ADC_MAX, 3) + "V)\n"
                        "• I : " + String(calib.zeroI) + " (" + String(calib.zeroI * VREF / ADC_MAX, 3) + "V)\n\n"
                        "💾 Data telah disimpan ke flash memory\n"
                        "🌐 Juga tersedia di web interface\n"
                        "✅ Sistem siap digunakan";
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, resultMsg, "Markdown", getCalibrationKeyboard(), true);
    }
    
    // ========== MANUAL VOLTAGE CALIBRATION ==========
    else if (text == "🔧 Kalibrasi V1") {
      String manualCal = "🔧 *KALIBRASI MANUAL - SUMBER 1*\n"
                        "━━━━━━━━━━━━━━━━━━\n"
                        "📝 *Langkah-langkah:*\n"
                        "1. Hubungkan tegangan ke Sumber 1\n"
                        "2. Ukur dengan multimeter\n"
                        "3. Kirim format: *CAL1:220.5*\n\n"
                        "*Contoh:*\n"
                        "• CAL1:220.0 → untuk 220V\n"
                        "• CAL1:227.3 → untuk 227.3V\n\n"
                        "📊 *Status saat ini:*\n"
                        "• Faktor Kalibrasi: " + String(calib.calibrationFactorV1, 4) + "\n"
                        "• Zero Point: " + String(calib.zeroV1) + "\n"
                        "• Terakhir diubah: " + getLastCalibrationTime();
      
      bot.sendMessage(CHAT_ID, manualCal, "Markdown");
    }
    
    else if (text == "🔧 Kalibrasi V2") {
      String manualCal = "🔧 *KALIBRASI MANUAL - SUMBER 2*\n"
                        "━━━━━━━━━━━━━━━━━━\n"
                        "📝 *Langkah-langkah:*\n"
                        "1. Hubungkan tegangan ke Sumber 2\n"
                        "2. Ukur dengan multimeter\n"
                        "3. Kirim format: *CAL2:220.5*\n\n"
                        "*Contoh:*\n"
                        "• CAL2:220.0 → untuk 220V\n"
                        "• CAL2:226.8 → untuk 226.8V\n\n"
                        "📊 *Status saat ini:*\n"
                        "• Faktor Kalibrasi: " + String(calib.calibrationFactorV2, 4) + "\n"
                        "• Zero Point: " + String(calib.zeroV1) + "\n"
                        "• Terakhir diubah: " + getLastCalibrationTime();
      
      bot.sendMessage(CHAT_ID, manualCal, "Markdown");
    }
    
    // ========== RESET ZERO POINTS ==========
    else if (text == "🔄 Reset All Zero") {
      String resetMsg = "🔄 *RESET SEMUA ZERO POINT*\n"
                       "━━━━━━━━━━━━━━━━━━\n"
                       "⚠️ *PERINGATAN:*\n"
                       "Ini akan mereset semua zero point ke default!\n\n"
                       "Konfirmasi dengan:\n"
                       "• *YES_RESET* → Reset semua\n"
                       "• *NO_CANCEL* → Batalkan";
      
      bot.sendMessage(CHAT_ID, resetMsg, "Markdown");
    }
    
    else if (text == "🎯 Reset Zero V1") {
      String confirmMsg = "🎯 *RESET ZERO POINT V1*\n"
                         "━━━━━━━━━━━━━━━━━━\n"
                         "Zero point saat ini: " + String(calib.zeroV1) + "\n"
                         "Konfirmasi reset?\n\n"
                         "• *YES_V1* → Reset V1\n"
                         "• *NO_CANCEL* → Batalkan";
      
      bot.sendMessage(CHAT_ID, confirmMsg, "Markdown");
    }
    
    else if (text == "🎯 Reset Zero V2") {
      String confirmMsg = "🎯 *RESET ZERO POINT V2*\n"
                         "━━━━━━━━━━━━━━━━━━\n"
                         "Zero point saat ini: " + String(calib.zeroV2) + "\n"
                         "Konfirmasi reset?\n\n"
                         "• *YES_V2* → Reset V2\n"
                         "• *NO_CANCEL* → Batalkan";
      
      bot.sendMessage(CHAT_ID, confirmMsg, "Markdown");
    }
    
    else if (text == "🎯 Reset Zero I") {
      String confirmMsg = "🎯 *RESET ZERO POINT I*\n"
                         "━━━━━━━━━━━━━━━━━━\n"
                         "Zero point saat ini: " + String(calib.zeroI) + "\n"
                         "Konfirmasi reset?\n\n"
                         "• *YES_I* → Reset I\n"
                         "• *NO_CANCEL* → Batalkan";
      
      bot.sendMessage(CHAT_ID, confirmMsg, "Markdown");
    }
    
    // ========== CALIBRATION COMMANDS ==========
    else if (text.startsWith("CAL1:")) {
      handleManualCalibration(1, text, CHAT_ID);
    }
    else if (text.startsWith("CAL2:")) {
      handleManualCalibration(2, text, CHAT_ID);
    }
    
    // ========== VERIFICATION & TESTING ==========
    else if (text == "📊 Cek Akurasi") {
      String testResult = "🎯 *TEST AKURASI SENSOR*\n"
                         "━━━━━━━━━━━━━━━━━━\n"
                         "🔄 Membaca sensor...";
      
      bot.sendMessage(CHAT_ID, testResult, "Markdown");
      
      delay(2000);
      
      // Baca sensor beberapa kali
      float sumV1 = 0, sumV2 = 0, sumI = 0;
      int readings = 5;
      
      for(int i=0; i<readings; i++) {
        readSensorsImproved();
        sumV1 += sensors.voltage1;
        sumV2 += sensors.voltage2;
        sumI += sensors.current;
        delay(1000);
      }
      
      float avgV1 = sumV1 / readings;
      float avgV2 = sumV2 / readings;
      float avgI = sumI / readings;
      
      String result = "✅ *HASIL TEST AKURASI*\n"
                     "━━━━━━━━━━━━━━━━━━\n"
                     "📊 *Rata-rata pembacaan:*\n"
                     "• V1: " + String(avgV1, 1) + " V\n"
                     "• V2: " + String(avgV2, 1) + " V\n"
                     "• I : " + String(avgI, 3) + " A\n\n"
                     "📈 *Stabilitas:*\n"
                     "• V1: ±" + String(getStability(sensors.voltage1), 1) + "V\n"
                     "• V2: ±" + String(getStability(sensors.voltage2), 1) + "V\n"
                     "• I : ±" + String(getStability(sensors.current), 3) + "A\n\n"
                     "💡 *Rekomendasi:*\n"
                     + getAccuracyRecommendation(avgV1, avgV2);
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, result, "Markdown", getCalibrationKeyboard(), true);
    }
    
    else if (text == "🔧 Test Sensor") {
      testSensorAccuracy();
      
      String testReport = "🧪 *TEST SENSOR COMPLETE*\n"
                         "━━━━━━━━━━━━━━━━━━\n"
                         "✅ Test telah selesai\n"
                         "📊 Lihat Serial Monitor untuk detail\n\n"
                         "Untuk hasil detail, kirim:\n"
                         "• \"📈 Test Report\" → Laporan lengkap\n"
                         "• \"🔍 Info Kalibrasi\" → Data kalibrasi\n"
                         "• *Web Interface:* http://" + WiFi.localIP().toString() + " → Kontrol penuh";
      
      bot.sendMessage(CHAT_ID, testReport, "Markdown");
    }
    
    else if (text == "📈 Test Report") {
      String report = generateTestReport();
      bot.sendMessage(CHAT_ID, report, "Markdown");
    }
    
    // ========== BACKUP & SAVE ==========
    else if (text == "💾 Backup Data") {
      String backup = generateBackupData();
      bot.sendMessage(CHAT_ID, backup, "Markdown");
    }
    
    else if (text == "💾 Simpan Data") {
      saveCalibration();
      
      String saveMsg = "💾 *DATA TERSIMPAN!*\n"
                      "━━━━━━━━━━━━━━━━━━\n"
                      "✅ Kalibrasi telah disimpan ke flash memory\n\n"
                      "*Data yang disimpan:*\n"
                      "• Zero Point V1, V2, I\n"
                      "• Faktor Kalibrasi V1, V2, I\n"
                      "• Mode AUTO/MANUAL\n"
                      "• Mode Maintenance\n"
                      "• Total Energi\n\n"
                      "📊 Data tidak akan hilang meski ESP32 mati\n"
                      "🌐 Juga tersedia di web interface";
      
      bot.sendMessage(CHAT_ID, saveMsg, "Markdown");
    }
    
    // ========== CALIBRATION INFO ==========
    else if (text == "🔍 Info Kalibrasi") {
      String calibInfo = generateCalibrationInfo();
      bot.sendMessage(CHAT_ID, calibInfo, "Markdown");
    }
    
    // ========== CONFIRMATION COMMANDS ==========
    else if (text == "YES_RESET") {
      resetAllCalibration();
      bot.sendMessage(CHAT_ID, "✅ Semua zero point telah direset!", "");
    }
    else if (text == "YES_V1") {
      recalibrateZeroPoint(1);
      bot.sendMessage(CHAT_ID, "✅ Zero point V1 telah direset!", "");
    }
    else if (text == "YES_V2") {
      recalibrateZeroPoint(2);
      bot.sendMessage(CHAT_ID, "✅ Zero point V2 telah direset!", "");
    }
    else if (text == "YES_I") {
      recalibrateZeroPoint(3);
      bot.sendMessage(CHAT_ID, "✅ Zero point I telah direset!", "");
    }
    else if (text == "NO_CANCEL") {
      bot.sendMessage(CHAT_ID, "❌ Operasi dibatalkan", "");
    }
    
    // ========== POWER CONTROL ==========
    else if (stringContains(text, "Sumber 1")) {
      if (autoMode) {
        bot.sendMessage(CHAT_ID, "❌ Mode AUTO aktif. Nonaktifkan dulu!", "");
      } else if (switching.inProgress) {
        bot.sendMessage(CHAT_ID, "⏳ Sistem sedang switching", "");
      } else if (maintenanceMode) {
        bot.sendMessage(CHAT_ID, "🔧 Maintenance mode aktif. Nonaktifkan dulu", "");
      } else {
        requestStateTransition(STATE_SOURCE1_ACTIVE, SWITCH_DELAY_NORMAL);
      }
    }
    else if (stringContains(text, "Sumber 2")) {
      if (autoMode) {
        bot.sendMessage(CHAT_ID, "❌ Mode AUTO aktif. Nonaktifkan dulu!", "");
      } else if (switching.inProgress) {
        bot.sendMessage(CHAT_ID, "⏳ Sistem sedang switching", "");
      } else if (maintenanceMode) {
        bot.sendMessage(CHAT_ID, "🔧 Maintenance mode aktif. Nonaktifkan dulu", "");
      } else {
        requestStateTransition(STATE_SOURCE2_ACTIVE, SWITCH_DELAY_NORMAL);
      }
    }
    else if (stringContains(text, "Matikan")) {
      if (autoMode) {
        bot.sendMessage(CHAT_ID, "❌ Mode AUTO aktif. Nonaktifkan dulu!", "");
      } else if (switching.inProgress) {
        bot.sendMessage(CHAT_ID, "⏳ Sistem sedang switching", "");
      } else {
        requestStateTransition(STATE_OFF, 1000);
      }
    }
    
    // ========== OTHER MENUS ==========
    else if (text == "⚙️ Settings") {
      String settings = "⚙️ *SYSTEM SETTINGS*\n";
      settings += "━━━━━━━━━━━━━━━━━━\n";
      settings += "🎛️ Auto Mode: " + String(autoMode ? "ENABLED" : "DISABLED") + "\n";
      settings += "🔧 Maintenance: " + String(maintenanceMode ? "ENABLED" : "DISABLED") + "\n";
      settings += "🌐 Web Interface: http://" + WiFi.localIP().toString() + "\n";
      settings += "📱 mDNS: http://powerswitch.local\n";
      settings += "━━━━━━━━━━━━━━━━━━\n";
      settings += "🔧 *Calibration Status:*\n";
      settings += getCalibrationStatus() + "\n";
      settings += "━━━━━━━━━━━━━━━━━━\n";
      settings += "Pilih opsi di bawah:";
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, settings, "Markdown", getSettingsKeyboard(), true);
    }
    else if (text == "📊 Status System") {
      String status = "📊 *STATUS SISTEM*\n";
      status += "━━━━━━━━━━━━━━━━━━\n";
      status += "🔌 Sumber 1: " + String(sensors.voltage1, 1) + " V\n";
      status += "🔌 Sumber 2: " + String(sensors.voltage2, 1) + " V\n";
      status += "🔋 Arus: " + String(sensors.current, 3) + " A\n";
      status += "💡 Daya: " + String(sensors.power, 1) + " W\n";
      status += "🔋 Energi: " + String(totalEnergy, 3) + " kWh\n";
      status += "⚡ Mode: " + String(autoMode ? "AUTO" : "MANUAL") + "\n";
      status += "🔧 Maintenance: " + String(maintenanceMode ? "ON" : "OFF") + "\n";
      status += "📡 Web Interface: http://" + WiFi.localIP().toString() + "\n";
      status += "━━━━━━━━━━━━━━━━━━\n";
      status += "🎛️ State: " + getStateText();
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, status, "Markdown", getMainMenuKeyboard(), true);
    }
    else if (text == "📈 Laporan") {
      String report = "📈 *SYSTEM REPORT*\n";
      report += "━━━━━━━━━━━━━━━━━━\n";
      report += "⏱️ Uptime: " + String(millis() / 3600000, 1) + " hours\n";
      report += "🔌 V1: " + String(sensors.voltage1, 1) + " V\n";
      report += "🔌 V2: " + String(sensors.voltage2, 1) + " V\n";
      report += "🔋 Current: " + String(sensors.current, 3) + " A\n";
      report += "💡 Power: " + String(sensors.power, 1) + " W\n";
      report += "🔋 Energy: " + String(totalEnergy, 3) + " kWh\n";
      report += "⚙️ Mode: " + String(autoMode ? "AUTO" : "MANUAL") + "\n";
      report += "📡 WiFi RSSI: " + String(WiFi.RSSI()) + " dBm\n";
      report += "🌐 Web Interface: http://" + WiFi.localIP().toString() + "\n";
      report += "👨‍💻 Developer: mh_alliwa";
      
      bot.sendMessageWithReplyKeyboard(CHAT_ID, report, "Markdown", getMainMenuKeyboard(), true);
    }
    else if (text == "🆘 Bantuan") {
      sendHelpMessage(CHAT_ID);
    }
    
    // ========== MODE CONTROL ==========
    else if (stringContains(text, "AUTO") || stringContains(text, "MANUAL")) {
      toggleAutoMode(CHAT_ID);
    }
    else if (stringContains(text, "Maintenance")) {
      toggleMaintenanceMode(CHAT_ID);
    }
  }
  
  lastTelegramUpdate = millis();
}

// ========== CALIBRATION HELPER FUNCTIONS ==========
void handleManualCalibration(int source, String text, String chat_id) {
  float realVoltage = text.substring(5).toFloat();
  
  if (realVoltage < 50 || realVoltage > 300) {
    bot.sendMessage(chat_id, "❌ *Tegangan tidak valid!*\n"
                    "Rentang: 50V - 300V\n"
                    "Contoh: CAL1:220.5", "Markdown");
    return;
  }
  
  String startMsg = "🔧 *MEMULAI KALIBRASI MANUAL*\n"
                   "━━━━━━━━━━━━━━━━━━\n"
                   "*Parameter:*\n"
                   "• Sumber: " + String(source) + "\n"
                   "• Tegangan Real: " + String(realVoltage, 1) + "V\n"
                   "• Estimasi: 10 detik\n\n"
                   "🔄 Proses dimulai...";
  
  bot.sendMessage(chat_id, startMsg, "Markdown");
  
  manualCalibrationWithRealVoltage(source, realVoltage);
  
  // Kirim hasil
  float errorPercent = calculateCalibrationError(source, realVoltage);
  
  String resultMsg = "✅ *KALIBRASI SELESAI!*\n"
                    "━━━━━━━━━━━━━━━━━━\n"
                    "*Hasil:*\n"
                    "• Input: " + String(realVoltage, 1) + "V\n"
                    "• Error: " + String(errorPercent, 1) + "%\n"
                    "• Status: " + getCalibrationQuality(errorPercent) + "\n\n"
                    "*Data Baru:*\n"
                    "• Zero Point: " + String((source==1)?calib.zeroV1:calib.zeroV2) + "\n"
                    "• Faktor: " + String((source==1)?calib.calibrationFactorV1:calib.calibrationFactorV2, 4) + "\n\n"
                    "💾 Data telah disimpan permanen\n"
                    "🌐 Juga tersedia di web interface";
  
  bot.sendMessageWithReplyKeyboard(chat_id, resultMsg, "Markdown", getCalibrationKeyboard(), true);
}

String generateCalibrationInfo() {
  String info = "🔧 *INFORMASI KALIBRASI*\n"
               "━━━━━━━━━━━━━━━━━━\n"
               "*Zero Points (ADC Value):*\n"
               "• V1: " + String(calib.zeroV1) + " (" + String(calib.zeroV1 * VREF / ADC_MAX, 3) + "V)\n"
               "• V2: " + String(calib.zeroV2) + " (" + String(calib.zeroV2 * VREF / ADC_MAX, 3) + "V)\n"
               "• I : " + String(calib.zeroI) + " (" + String(calib.zeroI * VREF / ADC_MAX, 3) + "V)\n\n"
               "*Calibration Factors:*\n"
               "• V1: " + String(calib.calibrationFactorV1, 6) + "\n"
               "• V2: " + String(calib.calibrationFactorV2, 6) + "\n"
               "• I : " + String(calib.calibrationFactorI, 6) + "\n\n"
               "*Status:*\n"
               "• Terakhir diubah: " + getLastCalibrationTime() + "\n"
               "• Auto-save: Aktif (5 menit)\n"
               "• Flash memory: OK\n"
               "• Web Interface: http://" + WiFi.localIP().toString() + "\n\n"
               "💡 *Tips:*\n"
               "• Error < 2% → Excellent\n"
               "• Error < 5% → Good\n"
               "• Error > 5% → Perlu kalibrasi ulang\n\n"
               "👨‍💻 Developer: mh_alliwa (@mh_alliwa)";
  
  return info;
}

String generateTestReport() {
  String report = "📈 *LAPORAN TEST SENSOR*\n"
                 "━━━━━━━━━━━━━━━━━━\n"
                 "*Kondisi Test:*\n"
                 "• Waktu: " + String(millis() / 1000) + " detik\n"
                 "• Mode: " + String(autoMode ? "AUTO" : "MANUAL") + "\n"
                 "• State: " + getStateText() + "\n"
                 "• IP: " + WiFi.localIP().toString() + "\n\n"
                 "*Pembacaan Saat Ini:*\n"
                 "• V1: " + String(sensors.voltage1, 1) + " V\n"
                 "• V2: " + String(sensors.voltage2, 1) + " V\n"
                 "• I : " + String(sensors.current, 3) + " A\n"
                 "• P : " + String(sensors.power, 1) + " W\n"
                 "• E : " + String(totalEnergy, 3) + " kWh\n\n"
                 "*Kalibrasi Aktif:*\n"
                 "• Faktor V1: " + String(calib.calibrationFactorV1, 4) + "\n"
                 "• Faktor V2: " + String(calib.calibrationFactorV2, 4) + "\n"
                 "• Zero V1: " + String(calib.zeroV1) + "\n"
                 "• Zero V2: " + String(calib.zeroV2) + "\n\n"
                 "*Rekomendasi:*\n";
  
  if (sensors.voltage1 < 10 && sensors.voltage2 < 10) {
    report += "✅ Sensor membaca nol dengan baik\n";
  } else if (abs(sensors.voltage1 - sensors.voltage2) > 20) {
    report += "⚠️ Perbedaan V1 dan V2 > 20V\n";
    report += "   Periksa koneksi atau kalibrasi\n";
  } else {
    report += "✅ Pembacaan stabil\n";
  }
  
  report += "\n🌐 *Web Interface:* http://" + WiFi.localIP().toString() + "\n";
  report += "👨‍💻 Developer: mh_alliwa (@mh_alliwa)";
  
  return report;
}

String generateBackupData() {
  String backup = "💾 *BACKUP DATA KALIBRASI*\n"
                 "━━━━━━━━━━━━━━━━━━\n"
                 "*SIMPAN DATA INI:*\n"
                 "```\n"
                 "Zero_V1 = " + String(calib.zeroV1) + "\n"
                 "Zero_V2 = " + String(calib.zeroV2) + "\n"
                 "Zero_I  = " + String(calib.zeroI) + "\n"
                 "Factor_V1 = " + String(calib.calibrationFactorV1, 6) + "\n"
                 "Factor_V2 = " + String(calib.calibrationFactorV2, 6) + "\n"
                 "Factor_I  = " + String(calib.calibrationFactorI, 6) + "\n"
                 "Auto_Mode = " + String(autoMode) + "\n"
                 "Maint_Mode = " + String(maintenanceMode) + "\n"
                 "Total_Energy = " + String(totalEnergy, 3) + "\n"
                 "```\n\n"
                 "*Cara Restore:*\n"
                 "1. Catat semua nilai di atas\n"
                 "2. Jika perlu restore, hubungi admin\n"
                 "3. Data bisa diinput manual via Serial\n"
                 "4. Atau gunakan web interface\n\n"
                 "*Web Interface:* http://" + WiFi.localIP().toString() + "\n"
                 "⏱️ Backup dibuat: " + String(millis() / 1000) + " detik sejak startup\n"
                 "👨‍💻 Developer: mh_alliwa (@mh_alliwa)";
  
  return backup;
}

// Helper functions
String getStateText() {
  switch (systemState) {
    case STATE_OFF: return "🔴 OFF";
    case STATE_SOURCE1_ACTIVE: return "🟢 SUMBER 1 AKTIF";
    case STATE_SOURCE2_ACTIVE: return "🟢 SUMBER 2 AKTIF";
    case STATE_SWITCHING: return "🟡 SEDANG SWITCHING";
    case STATE_FAULT: return "🔴 FAULT";
    default: return "⚪ UNKNOWN";
  }
}

String getCalibrationStatus() {
  if (calib.zeroV1 == 2048 && calib.zeroV2 == 2048) {
    return "🔴 PERLU KALIBRASI";
  } else if (abs(calib.calibrationFactorV1 - 1.0) > 0.2 || 
             abs(calib.calibrationFactorV2 - 1.0) > 0.2) {
    return "🟡 PERLU VERIFIKASI";
  } else {
    return "🟢 OK";
  }
}

String getDetailedCalibrationStatus() {
  String status = "";
  status += "• Zero V1: " + String(calib.zeroV1) + " (" + 
            String(calib.zeroV1 * VREF / ADC_MAX, 3) + "V)\n";
  status += "• Zero V2: " + String(calib.zeroV2) + " (" + 
            String(calib.zeroV2 * VREF / ADC_MAX, 3) + "V)\n";
  status += "• Zero I : " + String(calib.zeroI) + " (" + 
            String(calib.zeroI * VREF / ADC_MAX, 3) + "V)\n";
  status += "• Factor V1: " + String(calib.calibrationFactorV1, 4) + "\n";
  status += "• Factor V2: " + String(calib.calibrationFactorV2, 4) + "\n";
  status += "• Factor I : " + String(calib.calibrationFactorI, 4) + "\n\n";
  
  // Check status
  if (calib.zeroV1 == 2048) {
    status += "⚠️ V1 belum dikalibrasi\n";
  }
  if (calib.zeroV2 == 2048) {
    status += "⚠️ V2 belum dikalibrasi\n";
  }
  if (calib.calibrationFactorV1 < 0.8 || calib.calibrationFactorV1 > 1.2) {
    status += "⚠️ Factor V1 di luar range normal\n";
  }
  
  return status;
}

String getLastCalibrationTime() {
  preferences.begin("power-switch", true);
  unsigned long lastCalTime = preferences.getULong("lastCalTime", 0);
  preferences.end();
  
  if (lastCalTime == 0) {
    return "Belum pernah";
  }
  
  unsigned long secondsAgo = (millis() - lastCalTime) / 1000;
  
  if (secondsAgo < 60) {
    return String(secondsAgo) + " detik lalu";
  } else if (secondsAgo < 3600) {
    return String(secondsAgo / 60) + " menit lalu";
  } else if (secondsAgo < 86400) {
    return String(secondsAgo / 3600) + " jam lalu";
  } else {
    return String(secondsAgo / 86400) + " hari lalu";
  }
}

float calculateCalibrationError(int source, float realVoltage) {
  // Baca sensor setelah kalibrasi
  readSensorsImproved();
  
  float measuredVoltage = (source == 1) ? sensors.voltage1 : sensors.voltage2;
  float error = abs(realVoltage - measuredVoltage) / realVoltage * 100;
  
  return error;
}

String getCalibrationQuality(float errorPercent) {
  if (errorPercent < 1.0) return "⭐ EXCELLENT";
  else if (errorPercent < 3.0) return "✅ VERY GOOD";
  else if (errorPercent < 5.0) return "👍 GOOD";
  else if (errorPercent < 10.0) return "⚠️ FAIR";
  else return "❌ POOR";
}

String getAccuracyRecommendation(float v1, float v2) {
  String recommendation = "";
  
  if (v1 < 10 && v2 < 10) {
    recommendation = "✅ Sensor membaca nol dengan baik\n";
  } else if (abs(v1 - v2) > 30) {
    recommendation = "⚠️ Perbedaan V1 dan V2 > 30V\n";
    recommendation += "   Perlu kalibrasi manual\n";
  } else if (v1 > 250 || v2 > 250) {
    recommendation = "⚠️ Tegangan > 250V\n";
    recommendation += "   Periksa input tegangan\n";
  } else {
    recommendation = "✅ Pembacaan dalam range normal\n";
  }
  
  return recommendation;
}

float getStability(float value) {
  // Simple stability calculation
  static float lastValues[5] = {0};
  static int index = 0;
  
  lastValues[index] = value;
  index = (index + 1) % 5;
  
  float sum = 0;
  for(int i=0; i<5; i++) {
    sum += lastValues[i];
  }
  float avg = sum / 5;
  
  float variance = 0;
  for(int i=0; i<5; i++) {
    variance += pow(lastValues[i] - avg, 2);
  }
  
  return sqrt(variance / 5);
}

void resetAllCalibration() {
  calib.zeroV1 = 2048;
  calib.zeroV2 = 2048;
  calib.zeroI = 2048;
  calib.calibrationFactorV1 = 1.0;
  calib.calibrationFactorV2 = 1.0;
  calib.calibrationFactorI = 1.0;
  
  saveCalibration();
  
  Serial.println("✅ All calibration reset to default");
}

void sendHelpMessage(String chat_id) {
  String help = "🆘 *BANTUAN & CARA KALIBRASI*\n"
               "━━━━━━━━━━━━━━━━━━\n"
               "*KALIBRASI NOL (Tanpa Tegangan):*\n"
               "1. Pastikan TIDAK ADA tegangan\n"
               "2. Kirim: \"🎯 Auto Kalibrasi\"\n"
               "3. Tunggu ~15 detik\n\n"
               "*KALIBRASI DENGAN TEGANGAN:*\n"
               "1. Hubungkan tegangan (220V)\n"
               "2. Ukur dengan multimeter\n"
               "3. Kirim: \"CAL1:227.3\" (untuk V1)\n"
               "4. Atau: \"CAL2:226.8\" (untuk V2)\n\n"
               "*RESET ZERO POINT:*\n"
               "• \"🎯 Reset Zero V1\" - Reset V1\n"
               "• \"🎯 Reset Zero V2\" - Reset V2\n"
               "• \"🎯 Reset Zero I\" - Reset arus\n\n"
               "*TEST & VERIFIKASI:*\n"
               "• \"📊 Cek Akurasi\" - Test akurasi\n"
               "• \"🔍 Info Kalibrasi\" - Lihat data\n"
               "• \"💾 Backup Data\" - Backup ke Telegram\n\n"
               "*WEB INTERFACE:*\n"
               "• Buka: http://" + WiFi.localIP().toString() + "\n"
               "• Atau: http://powerswitch.local\n"
               "• Kontrol penuh dari browser\n\n"
               "*KONTAK DEVELOPER:*\n"
               "• Telegram: @mh_alliwa\n"
               "• Instagram: @mh.alliwa\n\n"
               "━━━━━━━━━━━━━━━━━━\n"
               "⚡ *POWER SWITCH SYSTEM V2*";
  
  bot.sendMessageWithReplyKeyboard(chat_id, help, "Markdown", getMainMenuKeyboard(), true);
}

void toggleAutoMode(String chat_id) {
  autoMode = !autoMode;
  saveCalibration();
  Blynk.virtualWrite(V1, autoMode ? 1 : 0);
  
  String msg = autoMode ? 
    "✅ *AUTO MODE ENABLED*\nSystem akan otomatis:\n• Switch ke Sumber 2 jika V1 < 180V\n• Kembali ke Sumber 1 jika V1 > 190V\n\n✅ Mode DISIMPAN PERMANEN" :
    "✅ *MANUAL MODE ENABLED*\nKontrol manual bebas\n\n✅ Mode DISIMPAN PERMANEN";
  
  bot.sendMessageWithReplyKeyboard(chat_id, msg, "Markdown", getMainMenuKeyboard(), true);
  logEvent(autoMode ? "Auto mode enabled" : "Manual mode enabled");
}

void toggleMaintenanceMode(String chat_id) {
  maintenanceMode = !maintenanceMode;
  saveCalibration();
  
  String msg = maintenanceMode ? 
    "🔧 *MAINTENANCE MODE ENABLED*\nSemua sumber dinonaktifkan\nSystem siap untuk maintenance\n\n✅ Mode DISIMPAN PERMANEN" :
    "🔧 *MAINTENANCE MODE DISABLED*\nOperasi normal dilanjutkan\n\n✅ Mode DISIMPAN PERMANEN";
  
  if (maintenanceMode && systemState != STATE_OFF) {
    requestStateTransition(STATE_OFF, 1000);
  }
  
  bot.sendMessageWithReplyKeyboard(chat_id, msg, "Markdown", getMainMenuKeyboard(), true);
  logEvent(maintenanceMode ? "Maintenance mode enabled" : "Maintenance mode disabled");
}

// ========== UTILITY FUNCTIONS ==========
void sendTelegramAlert(String message) {
  bot.sendMessage(CHAT_ID, message, "Markdown");
}

void logEvent(String event) {
  Serial.println("[LOG] " + event);
}

// ========== DISPLAY ==========
void updateDisplay() {
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay < 1000) return;
  lastDisplay = millis();
  
  lcd.clear();
  
  if (switching.inProgress) {
    lcd.setCursor(0, 0);
    lcd.print("SWITCHING...");
    lcd.setCursor(0, 1);
    lcd.print("Please wait");
  } else if (maintenanceMode) {
    lcd.setCursor(0, 0);
    lcd.print("MAINTENANCE MODE");
    lcd.setCursor(0, 1);
    lcd.print("System LOCKED");
  } else if (systemState == STATE_FAULT) {
    lcd.setCursor(0, 0);
    lcd.print("FAULT DETECTED!");
    lcd.setCursor(0, 1);
    lcd.print("Check Telegram");
  } else {
    lcd.setCursor(0, 0);
    if (systemState == STATE_OFF) {
      lcd.print("SYSTEM OFF");
    } else if (systemState == STATE_SOURCE1_ACTIVE) {
      lcd.print("SOURCE 1 ACTIVE");
    } else {
      lcd.print("SOURCE 2 ACTIVE");
    }
    
    lcd.setCursor(0, 1);
    lcd.printf("V1:%.0f V2:%.0f", sensors.voltage1, sensors.voltage2);
  }
  
  // Status LED blink
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
    lastBlink = millis();
  }
}

// ========== BLYNK INTERFACE ==========
BLYNK_CONNECTED() {
  Serial.println("✅ Blynk Connected!");
  Blynk.syncAll();
}

BLYNK_WRITE(V1) {
  bool newAutoMode = param.asInt();
  if (newAutoMode != autoMode) {
    autoMode = newAutoMode;
    saveCalibration();
    String msg = autoMode ? "Mode AUTO diaktifkan via Blynk" : "Mode MANUAL diaktifkan via Blynk";
    sendTelegramAlert(msg);
  }
}

BLYNK_WRITE(V8) {
  if (autoMode || switching.inProgress || maintenanceMode) {
    Blynk.virtualWrite(V8, (systemState == STATE_SOURCE1_ACTIVE));
    return;
  }
  
  bool btnState = param.asInt();
  if (btnState && systemState != STATE_SOURCE1_ACTIVE) {
    requestStateTransition(STATE_SOURCE1_ACTIVE, SWITCH_DELAY_NORMAL);
  } else if (!btnState && systemState == STATE_SOURCE1_ACTIVE) {
    requestStateTransition(STATE_OFF, 1000);
  }
}

BLYNK_WRITE(V9) {
  if (autoMode || switching.inProgress || maintenanceMode) {
    Blynk.virtualWrite(V9, (systemState == STATE_SOURCE2_ACTIVE));
    return;
  }
  
  bool btnState = param.asInt();
  if (btnState && systemState != STATE_SOURCE2_ACTIVE) {
    requestStateTransition(STATE_SOURCE2_ACTIVE, SWITCH_DELAY_NORMAL);
  } else if (!btnState && systemState == STATE_SOURCE2_ACTIVE) {
    requestStateTransition(STATE_OFF, 1000);
  }
}

void updateBlynkData() {
  Blynk.virtualWrite(V0, sensors.voltage1);
  Blynk.virtualWrite(V2, sensors.voltage2);
  Blynk.virtualWrite(V5, sensors.current);
  Blynk.virtualWrite(V6, sensors.power);
  Blynk.virtualWrite(V7, totalEnergy);
  Blynk.virtualWrite(V11, WiFi.RSSI());
}

// ========== TEST FUNCTION ==========
void testSensorAccuracy() {
  Serial.println("\n══════════════════════════════════");
  Serial.println("🧪 SENSOR ACCURACY TEST");
  Serial.println("══════════════════════════════════");
  
  // Test 1: No voltage
  Serial.println("\n1. TEST WITHOUT VOLTAGE:");
  Serial.println("   (Should read 0V or very close to 0V)");
  
  for (int i = 0; i < 5; i++) {
    readSensorsImproved();
    Serial.printf("   V1:%.1fV V2:%.1fV I:%.3fA\n", 
                  sensors.voltage1, sensors.voltage2, sensors.current);
    delay(1000);
  }
  
  // Test 2: Raw ADC values
  Serial.println("\n2. RAW ADC VALUES:");
  int rawV1 = analogRead(PIN_ZMPT1);
  int rawV2 = analogRead(PIN_ZMPT2);
  int rawI = analogRead(PIN_ACS);
  
  Serial.printf("   ZMPT1: %d (%.3fV)\n", rawV1, rawV1 * VREF / ADC_MAX);
  Serial.printf("   ZMPT2: %d (%.3fV)\n", rawV2, rawV2 * VREF / ADC_MAX);
  Serial.printf("   ACS712: %d (%.3fV)\n", rawI, rawI * VREF / ADC_MAX);
  Serial.printf("   Zero V1: %d Offset: %d\n", calib.zeroV1, rawV1 - calib.zeroV1);
  Serial.printf("   Zero V2: %d Offset: %d\n", calib.zeroV2, rawV2 - calib.zeroV2);
  Serial.printf("   Zero I: %d Offset: %d\n", calib.zeroI, rawI - calib.zeroI);
  
  // Test 3: Calibration factors
  Serial.println("\n3. CALIBRATION STATUS:");
  Serial.printf("   V1 Factor: %.4f\n", calib.calibrationFactorV1);
  Serial.printf("   V2 Factor: %.4f\n", calib.calibrationFactorV2);
  Serial.printf("   I Factor: %.4f\n", calib.calibrationFactorI);
  
  Serial.println("══════════════════════════════════\n");
}

// ========== SERIAL COMMANDS ==========
void checkSerialCommands() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "RESET_CAL") {
      Serial.println("⚠️ Resetting calibration to default...");
      calib.zeroV1 = 2048;
      calib.zeroV2 = 2048;
      calib.zeroI = 2048;
      calib.calibrationFactorV1 = 1.0;
      calib.calibrationFactorV2 = 1.0;
      calib.calibrationFactorI = 1.0;
      saveCalibration();
      Serial.println("✅ Calibration reset complete & SAVED!");
    }
    else if (command == "SAVE_CAL") {
      saveCalibration();
      Serial.println("✅ Calibration saved to flash!");
    }
    else if (command == "SHOW_CAL") {
      Serial.println("\n📊 CURRENT CALIBRATION (From Flash):");
      Serial.println("══════════════════════════════════");
      Serial.printf("Zero V1: %d (%.3fV)\n", calib.zeroV1, calib.zeroV1 * VREF / ADC_MAX);
      Serial.printf("Zero V2: %d (%.3fV)\n", calib.zeroV2, calib.zeroV2 * VREF / ADC_MAX);
      Serial.printf("Zero I : %d (%.3fV)\n", calib.zeroI, calib.zeroI * VREF / ADC_MAX);
      Serial.printf("Factor V1: %.6f\n", calib.calibrationFactorV1);
      Serial.printf("Factor V2: %.6f\n", calib.calibrationFactorV2);
      Serial.printf("Factor I : %.6f\n", calib.calibrationFactorI);
      Serial.println("══════════════════════════════════");
    }
    else if (command == "BACKUP_CAL") {
      backupCalibration();
    }
    else if (command == "FORCE_CAL") {
      Serial.println("⚠️ Starting forced calibration...");
      performAccurateCalibration();
    }
    else if (command == "STATUS") {
      Serial.println("\n📊 SYSTEM STATUS:");
      Serial.println("══════════════════════════════════");
      Serial.printf("State: %d\n", systemState);
      Serial.printf("Auto Mode: %s\n", autoMode ? "ON" : "OFF");
      Serial.printf("Maintenance: %s\n", maintenanceMode ? "ON" : "OFF");
      Serial.printf("V1: %.1fV\n", sensors.voltage1);
      Serial.printf("V2: %.1fV\n", sensors.voltage2);
      Serial.printf("I: %.3fA\n", sensors.current);
      Serial.printf("Power: %.1fW\n", sensors.power);
      Serial.printf("Energy: %.3fkWh\n", totalEnergy);
      Serial.printf("Web Interface: http://%s\n", WiFi.localIP().toString().c_str());
      Serial.printf("mDNS: http://powerswitch.local\n");
      Serial.println("══════════════════════════════════");
    }
    else if (command == "WEB_INFO") {
      Serial.println("\n🌐 WEB INTERFACE INFORMATION:");
      Serial.println("══════════════════════════════════");
      Serial.printf("Web Server: http://%s\n", WiFi.localIP().toString().c_str());
      Serial.printf("mDNS: http://powerswitch.local\n");
      Serial.printf("Port: %d\n", WEB_PORT);
      Serial.println("Features:");
      Serial.println("• Real-time monitoring");
      Serial.println("• Manual control");
      Serial.println("• Calibration management");
      Serial.println("• Backup download");
      Serial.println("• System restart");
      Serial.println("══════════════════════════════════");
    }
  }
}

// ========== SETUP ==========
void setup() {
  initHardware();
  
  // TAMPILKAN LOADING
  lcd.clear();
  lcd.print("LOADING SYSTEM...");
  lcd.setCursor(0, 1);
  lcd.print("Developer:mh_alliwa");
  
  // Load calibration dari flash memory (TIDAK KALIBRASI ULANG)
  loadCalibration();
  
  // Cek jika kalibrasi belum pernah dilakukan (pertama kali)
  if (calib.zeroV1 == 2048 && calib.zeroV2 == 2048 && calib.zeroI == 2048) {
    Serial.println("\n⚠️ FIRST TIME SETUP - No calibration found");
    lcd.clear();
    lcd.print("FIRST TIME SETUP");
    lcd.setCursor(0, 1);
    lcd.print("Calibration needed");
    delay(3000);
    
    // Tanya user via Serial Monitor
    Serial.println("\n══════════════════════════════════");
    Serial.println("   FIRST TIME CALIBRATION");
    Serial.println("   Developer: mh_alliwa");
    Serial.println("══════════════════════════════════");
    Serial.println("System needs initial calibration.");
    Serial.println("Options:");
    Serial.println("1. Send 'FORCE_CAL' to calibrate now");
    Serial.println("2. Send 'SKIP_CAL' to skip for now");
    Serial.println("3. Wait 10 seconds to auto-calibrate");
    Serial.println("══════════════════════════════════");
    
    unsigned long startTime = millis();
    bool calibrated = false;
    
    while (millis() - startTime < 10000 && !calibrated) {
      if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "FORCE_CAL") {
          performAccurateCalibration();
          calibrated = true;
        } 
        else if (command == "SKIP_CAL") {
          Serial.println("⚠️ Skipping calibration. System may not work properly!");
          lcd.clear();
          lcd.print("CALIBRATION");
          lcd.setCursor(0, 1);
          lcd.print("SKIPPED!");
          delay(2000);
          calibrated = true;
        }
      }
      delay(100);
    }
    
    // Jika tidak ada input dalam 10 detik, auto calibrate
    if (!calibrated) {
      Serial.println("\n⏱️ No input received. Auto-calibrating...");
      performAccurateCalibration();
    }
  } else {
    Serial.println("\n✅ Using saved calibration from flash");
    lcd.clear();
    lcd.print("CALIBRATION");
    lcd.setCursor(0, 1);
    lcd.print("LOADED FROM MEM");
    delay(2000);
  }
  
  // WiFi Connection
  lcd.clear();
  lcd.print("Connecting WiFi");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 30) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(timeout % 16, 1);
    lcd.print(".");
    timeout++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.println("📡 IP: " + WiFi.localIP().toString());
    
    lcd.clear();
    lcd.print("WiFi Connected!");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP().toString());
    
    client.setInsecure();
    Blynk.config(BLYNK_AUTH_TOKEN);
    
    // Initialize Web Server
    initWebServer();
    
    delay(2000);
    
    String startupMsg = "🚀 *SYSTEM ONLINE V2*\n";
    startupMsg += "IP: " + WiFi.localIP().toString() + "\n";
    startupMsg += "mDNS: powerswitch.local\n";
    startupMsg += "Status: SYSTEM OFF\n";
    startupMsg += "Mode: " + String(autoMode ? "AUTO" : "MANUAL") + "\n";
    startupMsg += "Calibration: LOADED FROM FLASH\n";
    startupMsg += "Web Interface: ACTIVE\n";
    startupMsg += "━━━━━━━━━━━━━━━━━━\n";
    startupMsg += "Developer: mh_alliwa (@mh_alliwa)\n";
    startupMsg += "Kirim /start untuk menu";
    
    bot.sendMessageWithReplyKeyboard(CHAT_ID, startupMsg, "Markdown", getMainMenuKeyboard(), true);
    
  } else {
    Serial.println("\n❌ WiFi Connection Failed!");
    lcd.clear();
    lcd.print("WiFi FAILED!");
    lcd.setCursor(0, 1);
    lcd.print("Offline Mode");
  }
  
  // Setup timers
  timer.setInterval(500L, updateBlynkData);
  timer.setInterval(1000L, updateDisplay);
  
  delay(2000);
  lcd.clear();
  
  // Tampilkan startup complete
  lcd.clear();
  lcd.print("SYSTEM READY");
  lcd.setCursor(0, 1);
  lcd.print("Web+Telegram ACTIVE");
  delay(2000);
}

// ========== LOOP ==========
void loop() {
  Blynk.run();
  timer.run();
  
  readSensorsImproved();
  handleStateMachine();
  handleTelegramCommands();
  handleWebClients();  // NEW: Handle web server
  checkSerialCommands();  // Untuk kontrol via Serial Monitor
  
  // Periodic auto-save (setiap 5 menit)
  static unsigned long lastSave = 0;
  if (millis() - lastSave > 300000) {
    lastSave = millis();
    saveCalibration();
    Serial.println("💾 Settings auto-saved to flash");
  }
  
  // Periodic status update (setiap 30 detik)
  static unsigned long lastStatus = 0;
  if (millis() - lastStatus > 30000) { 
    lastStatus = millis();
    Serial.println("✅ System running with web interface");
    Serial.println("🌐 Web Interface: http://" + WiFi.localIP().toString());
  }
}
