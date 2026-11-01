#include "ParkingManager.h"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace {
std::string readLine(const std::string& prompt) {
    std::cout << prompt; std::string value; std::getline(std::cin, value); return value;
}
int readChoice() {
    std::string raw = readLine("Select an option: ");
    try {
        std::size_t used=0; int value=std::stoi(raw,&used);
        if (used != raw.size()) throw std::invalid_argument("extra");
        return value;
    } catch (...) { std::cout << "Invalid menu input. Enter a number.\n"; return -1; }
}
void printBanner() {
    std::cout << "\n====================================================\n"
              << "           SMART PARKING MANAGEMENT SYSTEM\n"
              << "       C++17 | OOP | File-based Persistence\n"
              << "====================================================\n";
}

// Writes the ParkPilot web dashboard (single offline HTML file) and opens it
// in the default browser. The C++ backend remains the source of truth; the
// dashboard is a browser-side prototype that mirrors the same logic and keeps
// its own data in localStorage (key 'parkpilot_cpp').
void generateAndLaunchDashboard() {
    const std::string fileName = "parkpilot_dashboard.html";
    const char* html = R"HTML_DASH(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ParkPilot | Smart Parking Management System</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Bricolage+Grotesque:wght@400;600;800&family=IBM+Plex+Mono:wght@400;500;600&display=swap" rel="stylesheet">
<style>
:root{
  --bg:#0b1220; --panel:#111a2e; --panel2:#16223b; --line:#23324f;
  --text:#e6ecf8; --muted:#8c9bb8; --sign:#2f7bff; --sign-d:#1d56c2;
  --free:#1fbf75; --free-bg:rgba(31,191,117,.12); --busy:#ef4a5a; --busy-bg:rgba(239,74,90,.14);
  --amber:#f5b83d;
  --sans:'Bricolage Grotesque',system-ui,Segoe UI,Arial,sans-serif;
  --mono:'IBM Plex Mono',Consolas,Menlo,monospace;
}
*{box-sizing:border-box}
html,body{margin:0;height:100%}
body{background:var(--bg);color:var(--text);font-family:var(--sans);font-size:15px;line-height:1.5}
button,input,select{font:inherit;color:inherit}
:focus-visible{outline:2px solid var(--sign);outline-offset:2px}

/* Header */
header{display:flex;align-items:center;gap:16px;padding:14px 24px;background:var(--panel);border-bottom:1px solid var(--line);position:sticky;top:0;z-index:5}
.logo{width:38px;height:38px;border-radius:9px;background:var(--sign);display:grid;place-items:center;font-weight:800;font-size:22px;color:#fff}
header h1{margin:0;font-size:19px;font-weight:600;letter-spacing:.2px}
header h1 small{display:block;font-size:12px;font-weight:400;color:var(--muted)}
.badge{margin-left:auto;font-family:var(--mono);font-size:12px;padding:4px 10px;border:1px solid var(--sign);color:#9cc2ff;border-radius:999px}

/* Layout */
.shell{display:grid;grid-template-columns:220px 1fr;min-height:calc(100% - 67px)}
nav{background:var(--panel);border-right:1px solid var(--line);padding:18px 12px;display:flex;flex-direction:column;gap:4px}
nav button{display:flex;align-items:center;gap:10px;text-align:left;background:none;border:0;padding:10px 12px;border-radius:8px;color:var(--muted);cursor:pointer}
nav button:hover{background:var(--panel2);color:var(--text)}
nav button.active{background:var(--sign);color:#fff;font-weight:600}
nav .spacer{flex:1}
nav .tools{display:flex;flex-direction:column;gap:8px;border-top:1px solid var(--line);padding-top:14px;margin-top:10px}
main{padding:24px;max-width:1180px;width:100%}
.tab{display:none}.tab.active{display:block}
h2{margin:0 0 4px;font-size:24px;font-weight:800}
.sub{color:var(--muted);margin:0 0 20px}

/* Cards */
.cards{display:grid;grid-template-columns:repeat(4,1fr);gap:14px;margin-bottom:24px}
.card{background:var(--panel);border:1px solid var(--line);border-radius:12px;padding:16px 18px}
.card .k{color:var(--muted);font-size:13px}
.card .v{font-family:var(--mono);font-size:30px;font-weight:600;margin-top:4px}
.card.free .v{color:var(--free)}.card.busy .v{color:var(--busy)}.card.rev .v{color:var(--amber)}

/* Slot map */
.panel{background:var(--panel);border:1px solid var(--line);border-radius:12px;padding:18px}
.panel h3{margin:0 0 12px;font-size:16px;font-weight:600}
.legend{display:flex;gap:16px;color:var(--muted);font-size:13px;margin-bottom:12px}
.dot{display:inline-block;width:10px;height:10px;border-radius:3px;margin-right:6px}
.grid{display:grid;grid-template-columns:repeat(6,1fr);gap:10px}
.slot{border-radius:10px;padding:12px 8px;min-height:84px;cursor:pointer;text-align:center;border:1.5px solid;display:flex;flex-direction:column;justify-content:center;gap:4px;transition:transform .12s}
.slot:hover{transform:translateY(-2px)}
.slot .n{font-size:12px;color:var(--muted)}
.slot .r{font-family:var(--mono);font-size:12px;font-weight:600;word-break:break-all}
.slot.free{background:var(--free-bg);border-color:var(--free)}
.slot.free .r{color:var(--free)}
.slot.busy{background:var(--busy-bg);border-color:var(--busy)}
.slot.busy .r{color:#ffb3ba}

/* Forms */
.form{display:grid;gap:14px;max-width:460px}
label{display:grid;gap:6px;color:var(--muted);font-size:13px}
input,select{background:var(--bg);border:1px solid var(--line);border-radius:8px;padding:10px 12px;color:var(--text);width:100%}
input.plate{font-family:var(--mono);letter-spacing:1px;text-transform:uppercase}
select option{background:var(--panel)}
.btn{background:var(--sign);border:0;color:#fff;font-weight:600;padding:10px 16px;border-radius:8px;cursor:pointer}
.btn:hover{background:var(--sign-d)}
.btn.ghost{background:transparent;border:1px solid var(--line);color:var(--text)}
.btn.ghost:hover{background:var(--panel2)}
.btn.danger{background:var(--busy)}
.btn.danger:hover{background:#c93a49}
.row{display:flex;gap:10px;flex-wrap:wrap;align-items:end}
.row label{flex:1;min-width:200px}
.msg{padding:10px 14px;border-radius:8px;font-size:14px;margin-top:4px;display:none}
.msg.ok{display:block;background:var(--free-bg);border:1px solid var(--free);color:#8ff0c0}
.msg.err{display:block;background:var(--busy-bg);border:1px solid var(--busy);color:#ffb3ba}
.split{display:grid;grid-template-columns:minmax(300px,460px) 1fr;gap:24px;align-items:start}
.rates{font-size:13px;color:var(--muted);margin-top:10px}
.rates span{font-family:var(--mono);color:var(--text)}

/* Details + receipt */
.kv{display:grid;grid-template-columns:140px 1fr;gap:6px 12px;margin:12px 0}
.kv dt{color:var(--muted)}.kv dd{margin:0;font-family:var(--mono)}
.receipt{background:#fff;color:#111;border-radius:6px;padding:22px 26px;font-family:var(--mono);font-size:13px;max-width:380px}
.receipt h4{margin:0 0 2px;text-align:center;font-family:var(--sans);font-size:18px}
.receipt .c{text-align:center;color:#555;margin-bottom:10px}
.receipt hr{border:0;border-top:1px dashed #888;margin:10px 0}
.receipt .line{display:flex;justify-content:space-between;gap:12px;padding:2px 0}
.receipt .tot{font-size:16px;font-weight:600}
.empty{color:var(--muted);padding:18px;border:1px dashed var(--line);border-radius:10px;text-align:center}

/* Tables */
.tw{overflow-x:auto}
table{width:100%;border-collapse:collapse;font-size:14px}
th,td{text-align:left;padding:9px 12px;border-bottom:1px solid var(--line);white-space:nowrap}
th{color:var(--muted);font-weight:500;font-size:13px}
td.m{font-family:var(--mono)}
tr.hit td{background:rgba(47,123,255,.1)}
.stack{display:grid;gap:20px}
.bars{display:grid;gap:10px;margin-top:8px}
.bar{display:grid;grid-template-columns:100px 1fr 90px;gap:10px;align-items:center;font-size:13px}
.bar .t{height:10px;background:var(--bg);border-radius:5px;overflow:hidden}
.bar .t i{display:block;height:100%;background:var(--sign)}
.bar b{font-family:var(--mono);text-align:right;font-weight:500}

@media(max-width:900px){
  .shell{grid-template-columns:1fr}
  nav{flex-direction:row;flex-wrap:wrap;border-right:0;border-bottom:1px solid var(--line)}
  nav .spacer{display:none}
  nav .tools{flex-direction:row;border:0;padding:0;margin:0}
  .cards{grid-template-columns:repeat(2,1fr)}
  .grid{grid-template-columns:repeat(3,1fr)}
  .split{grid-template-columns:1fr}
}
@media print{
  body *{visibility:hidden}
  #receiptBox,#receiptBox *{visibility:visible}
  #receiptBox{position:absolute;left:0;top:0;width:100%;max-width:none;border-radius:0}
}
@media(prefers-reduced-motion:reduce){.slot{transition:none}}
</style>
</head>
<body>
<header>
  <div class="logo" aria-hidden="true">P</div>
  <h1>Smart Parking Management System<small>ParkPilot Web Dashboard</small></h1>
  <span class="badge">C++17</span>
</header>

<div class="shell">
  <nav aria-label="Main">
    <button class="active" data-tab="dashboard">Dashboard</button>
    <button data-tab="entry">Vehicle Entry</button>
    <button data-tab="exit">Vehicle Exit</button>
    <button data-tab="records">Records</button>
    <button data-tab="reports">Reports</button>
    <div class="spacer"></div>
    <div class="tools">
      <button class="btn ghost" id="demoBtn" type="button">Load demo data</button>
      <button class="btn ghost" id="resetBtn" type="button">Clear all data</button>
    </div>
  </nav>

  <main>
    <!-- DASHBOARD -->
    <section class="tab active" id="tab-dashboard">
      <h2>Dashboard</h2>
      <p class="sub">Live view of the lot. Select an occupied slot to check that vehicle out.</p>
      <div class="cards">
        <div class="card"><div class="k">Total slots</div><div class="v" id="cTotal">20</div></div>
        <div class="card free"><div class="k">Available</div><div class="v" id="cFree">20</div></div>
        <div class="card busy"><div class="k">Occupied</div><div class="v" id="cBusy">0</div></div>
        <div class="card rev"><div class="k">Revenue (Rs.)</div><div class="v" id="cRev">0.00</div></div>
      </div>
      <div class="panel">
        <h3>Live parking map</h3>
        <div class="legend">
          <span><i class="dot" style="background:var(--free)"></i>Available</span>
          <span><i class="dot" style="background:var(--busy)"></i>Occupied</span>
        </div>
        <div class="grid" id="slotGrid"></div>
      </div>
    </section>

    <!-- ENTRY -->
    <section class="tab" id="tab-entry">
      <h2>Vehicle entry</h2>
      <p class="sub">Registers the vehicle and assigns the first free slot.</p>
      <div class="split">
        <div class="panel">
          <div class="form">
            <label>Registration number
              <input class="plate" id="eReg" placeholder="JH01AB1234" maxlength="14" autocomplete="off">
            </label>
            <label>Vehicle type
              <select id="eType">
                <option>Car</option><option>Motorcycle</option><option>Truck</option>
              </select>
            </label>
            <label>Owner name
              <input id="eOwner" placeholder="Rahul Kumar" autocomplete="off">
            </label>
            <button class="btn" id="parkBtn" type="button">Park vehicle</button>
            <div class="msg" id="eMsg" role="status"></div>
          </div>
        </div>
        <div class="panel">
          <h3>Hourly tariff</h3>
          <p class="rates">Any started hour is charged in full; minimum one hour.</p>
          <div class="bars" id="rateList"></div>
        </div>
      </div>
    </section>

    <!-- EXIT -->
    <section class="tab" id="tab-exit">
      <h2>Vehicle exit</h2>
      <p class="sub">Look up a parked vehicle, review the fee and issue the receipt.</p>
      <div class="split">
        <div class="panel">
          <div class="row">
            <label>Registration number
              <input class="plate" id="xReg" placeholder="JH01AB1234" maxlength="14" autocomplete="off">
            </label>
            <button class="btn" id="findBtn" type="button">Find vehicle</button>
          </div>
          <div class="msg" id="xMsg" role="status"></div>
          <div id="xInfo"></div>
        </div>
        <div>
          <div id="receiptWrap" class="empty">The receipt will appear here after checkout.</div>
        </div>
      </div>
    </section>

    <!-- RECORDS -->
    <section class="tab" id="tab-records">
      <h2>Records</h2>
      <p class="sub">Search by registration number or owner name.</p>
      <div class="stack">
        <label style="max-width:360px">Search
          <input id="qBox" placeholder="Type to filter records" autocomplete="off">
        </label>
        <div class="panel"><h3>Active sessions</h3><div class="tw" id="activeTbl"></div></div>
        <div class="panel"><h3>Parking history</h3><div class="tw" id="histTbl"></div></div>
      </div>
    </section>

    <!-- REPORTS -->
    <section class="tab" id="tab-reports">
      <h2>Reports</h2>
      <p class="sub">Revenue from completed sessions.</p>
      <div class="stack">
        <div class="panel"><h3>Revenue by vehicle type</h3><div class="bars" id="revBars"></div></div>
        <div class="panel"><h3>Revenue log</h3><div class="tw" id="revTbl"></div></div>
      </div>
    </section>
  </main>
</div>

<script>
(function(){
'use strict';
var KEY='parkpilot_cpp';
var TOTAL=20;
var RATES={Car:30,Motorcycle:15,Truck:60}; /* mirrors Vehicle.cpp hourlyRate() */
var state=load();
var currentExit=null;

function fresh(){return {active:[],history:[]};}
function load(){
  try{
    var s=JSON.parse(localStorage.getItem(KEY));
    if(s&&Array.isArray(s.active)&&Array.isArray(s.history))return s;
  }catch(e){}
  return fresh();
}
function save(){try{localStorage.setItem(KEY,JSON.stringify(state));}catch(e){}}
function $(id){return document.getElementById(id);}
function esc(s){return String(s).replace(/[&<>"']/g,function(c){return {'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c];});}
function money(n){return 'Rs. '+Number(n).toFixed(2);}
function when(ts){return new Date(ts).toLocaleString('en-IN',{day:'2-digit',month:'short',hour:'2-digit',minute:'2-digit'});}
function dur(mins){var h=Math.floor(mins/60),m=mins%60;return (h?h+' h ':'')+m+' min';}
function cleanReg(v){return v.toUpperCase().replace(/[^A-Z0-9]/g,'');}

/* ---- Core logic (mirrors ParkingManager.cpp) ---- */
function findActive(reg){for(var i=0;i<state.active.length;i++){if(state.active[i].reg===reg)return state.active[i];}return null;}
function slotOwner(id){for(var i=0;i<state.active.length;i++){if(state.active[i].slot===id)return state.active[i];}return null;}
function calcFee(entry,exit,type){
  var ms=Math.max(0,exit-entry);
  var mins=Math.floor(ms/60000);
  var hours=Math.max(1,Math.ceil(ms/3600000));
  return {mins:mins,hours:hours,fee:hours*RATES[type]};
}
function parkVehicle(type,reg,owner,entryTs){
  if(!reg||!owner)throw new Error('Registration and owner are required.');
  if(!RATES.hasOwnProperty(type))throw new Error('Unknown vehicle type. Use Car, Motorcycle, or Truck.');
  if(findActive(reg))throw new Error('TC-05: Vehicle with same registration already parked.');
  var slot=0;
  for(var i=1;i<=TOTAL;i++){if(!slotOwner(i)){slot=i;break;}}
  if(!slot)throw new Error('No slot is available. The lot is full.');
  state.active.push({reg:reg,type:type,owner:owner,slot:slot,entry:entryTs||Date.now()});
  save();
  return slot;
}
function exitVehicle(reg){
  var a=findActive(reg);
  if(!a)throw new Error('No active parking record found for this registration.');
  var now=Date.now(),c=calcFee(a.entry,now,a.type);
  var rec={no:'R'+now.toString().slice(-8),reg:a.reg,type:a.type,owner:a.owner,slot:a.slot,mins:c.mins,hours:c.hours,fee:c.fee,entry:a.entry,exit:now};
  state.history.push(rec);
  state.active=state.active.filter(function(x){return x.reg!==reg;});
  save();
  return rec;
}

/* ---- Messages ---- */
function say(id,text,ok){var el=$(id);el.className='msg '+(ok?'ok':'err');el.textContent=text;}
function clearMsg(id){var el=$(id);el.className='msg';el.textContent='';}

/* ---- Tabs ---- */
function showTab(name){
  var tabs=document.querySelectorAll('.tab');
  for(var i=0;i<tabs.length;i++)tabs[i].classList.toggle('active',tabs[i].id==='tab-'+name);
  var btns=document.querySelectorAll('nav button[data-tab]');
  for(var j=0;j<btns.length;j++)btns[j].classList.toggle('active',btns[j].getAttribute('data-tab')===name);
  renderAll();
}

/* ---- Rendering ---- */
function renderCards(){
  var busy=state.active.length,rev=0;
  state.history.forEach(function(h){rev+=h.fee;});
  $('cTotal').textContent=TOTAL;
  $('cFree').textContent=TOTAL-busy;
  $('cBusy').textContent=busy;
  $('cRev').textContent=rev.toFixed(2);
}
function renderMap(){
  var html='';
  for(var i=1;i<=TOTAL;i++){
    var a=slotOwner(i);
    if(a){
      html+='<button type="button" class="slot busy" data-slot="'+i+'" data-reg="'+esc(a.reg)+'" aria-label="Slot '+i+' occupied by '+esc(a.reg)+'"><span class="n">Slot '+i+'</span><span class="r">'+esc(a.reg)+'</span></button>';
    }else{
      html+='<button type="button" class="slot free" data-slot="'+i+'" aria-label="Slot '+i+' available"><span class="n">Slot '+i+'</span><span class="r">Free</span></button>';
    }
  }
  $('slotGrid').innerHTML=html;
}
function renderRates(){
  var max=0,k,h='';
  for(k in RATES)if(RATES[k]>max)max=RATES[k];
  for(k in RATES)h+='<div class="bar"><span>'+k+'</span><span class="t"><i style="width:'+(RATES[k]/max*100)+'%"></i></span><b>Rs. '+RATES[k]+'/hr</b></div>';
  $('rateList').innerHTML=h;
}
function table(cols,rows,emptyText){
  if(!rows.length)return '<div class="empty">'+emptyText+'</div>';
  var h='<table><thead><tr>';
  cols.forEach(function(c){h+='<th>'+c+'</th>';});
  h+='</tr></thead><tbody>'+rows.join('')+'</tbody></table>';
  return h;
}
function renderRecords(){
  var q=$('qBox').value.trim().toUpperCase();
  function match(r){return !q||r.reg.indexOf(q)>-1||r.owner.toUpperCase().indexOf(q)>-1;}
  var act=state.active.filter(match).map(function(a){
    return '<tr'+(q?' class="hit"':'')+'><td class="m">'+esc(a.reg)+'</td><td>'+esc(a.type)+'</td><td>'+esc(a.owner)+'</td><td class="m">'+a.slot+'</td><td>'+when(a.entry)+'</td></tr>';
  });
  $('activeTbl').innerHTML=table(['Registration','Type','Owner','Slot','Entered'],act,q?'No active session matches your search.':'No vehicles are parked right now.');
  var his=state.history.filter(match).slice().reverse().map(function(h){
    return '<tr'+(q?' class="hit"':'')+'><td class="m">'+esc(h.no)+'</td><td class="m">'+esc(h.reg)+'</td><td>'+esc(h.type)+'</td><td>'+esc(h.owner)+'</td><td class="m">'+h.slot+'</td><td>'+dur(h.mins)+'</td><td class="m">'+money(h.fee)+'</td></tr>';
  });
  $('histTbl').innerHTML=table(['Receipt','Registration','Type','Owner','Slot','Duration','Fee'],his,q?'No completed session matches your search.':'No completed sessions yet.');
}
function renderReports(){
  var totals={},sum=0;
  Object.keys(RATES).forEach(function(k){totals[k]=0;});
  state.history.forEach(function(h){totals[h.type]=(totals[h.type]||0)+h.fee;sum+=h.fee;});
  var max=0,k;
  for(k in totals)if(totals[k]>max)max=totals[k];
  var b='';
  for(k in totals)b+='<div class="bar"><span>'+esc(k)+'</span><span class="t"><i style="width:'+(max?totals[k]/max*100:0)+'%"></i></span><b>'+money(totals[k])+'</b></div>';
  b+='<div class="bar"><span><strong>Total</strong></span><span></span><b>'+money(sum)+'</b></div>';
  $('revBars').innerHTML=b;
  var rows=state.history.slice().reverse().map(function(h){
    return '<tr><td class="m">'+esc(h.no)+'</td><td>'+when(h.exit)+'</td><td class="m">'+esc(h.reg)+'</td><td>'+esc(h.type)+'</td><td class="m">'+money(h.fee)+'</td></tr>';
  });
  $('revTbl').innerHTML=table(['Receipt','Exit time','Registration','Type','Amount'],rows,'No revenue recorded yet. Complete a vehicle exit to see entries here.');
}
function renderExitInfo(){
  var box=$('xInfo');
  if(!currentExit){box.innerHTML='';return;}
  var a=findActive(currentExit);
  if(!a){box.innerHTML='';currentExit=null;return;}
  var c=calcFee(a.entry,Date.now(),a.type);
  box.innerHTML='<dl class="kv"><dt>Registration</dt><dd>'+esc(a.reg)+'</dd><dt>Type</dt><dd>'+esc(a.type)+' (Rs. '+RATES[a.type]+'/hr)</dd><dt>Owner</dt><dd>'+esc(a.owner)+'</dd><dt>Slot</dt><dd>'+a.slot+'</dd><dt>Entered</dt><dd>'+when(a.entry)+'</dd><dt>Duration</dt><dd>'+dur(c.mins)+' ('+c.hours+' h billed)</dd><dt>Fee due</dt><dd style="color:var(--amber);font-size:18px">'+money(c.fee)+'</dd></dl><button class="btn danger" id="confirmExit" type="button">Confirm exit and issue receipt</button>';
  $('confirmExit').onclick=doExit;
}
function showReceipt(r){
  $('receiptWrap').className='';
  $('receiptWrap').innerHTML='<div class="receipt" id="receiptBox"><h4>ParkPilot</h4><div class="c">Smart Parking Receipt</div><hr>'+
    line('Receipt no.',r.no)+line('Registration',r.reg)+line('Vehicle',r.type)+line('Owner',r.owner)+line('Slot',r.slot)+
    '<hr>'+line('Entry',when(r.entry))+line('Exit',when(r.exit))+line('Duration',dur(r.mins))+line('Billed hours',r.hours)+line('Rate','Rs. '+RATES[r.type]+'/hr')+
    '<hr><div class="line tot"><span>TOTAL</span><span>'+money(r.fee)+'</span></div><hr><div class="c">Thank you. Drive safe.</div></div>'+
    '<p><button class="btn" id="printBtn" type="button">Print receipt</button></p>';
  $('printBtn').onclick=function(){window.print();};
}
function line(a,b){return '<div class="line"><span>'+a+'</span><span>'+esc(b)+'</span></div>';}
function renderAll(){renderCards();renderMap();renderRecords();renderReports();renderExitInfo();}

/* ---- Actions ---- */
function doPark(){
  var reg=cleanReg($('eReg').value),owner=$('eOwner').value.replace(/[,\r\n]/g,' ').trim(),type=$('eType').value;
  try{
    var slot=parkVehicle(type,reg,owner);
    say('eMsg','SUCCESS: '+reg+' allocated slot '+slot+'.',true);
    $('eReg').value='';$('eOwner').value='';
    renderAll();
  }catch(e){say('eMsg',e.message,false);}
}
function lookup(reg){
  reg=cleanReg(reg);
  $('xReg').value=reg;
  if(!reg){say('xMsg','Enter a registration number.',false);return;}
  if(!findActive(reg)){currentExit=null;say('xMsg','No active parking record found for this registration.',false);renderExitInfo();return;}
  currentExit=reg;clearMsg('xMsg');renderExitInfo();
}
function doExit(){
  if(!currentExit)return;
  try{
    var r=exitVehicle(currentExit);
    currentExit=null;
    say('xMsg','Checkout complete. Receipt '+r.no+' issued.',true);
    showReceipt(r);
    renderAll();
  }catch(e){say('xMsg',e.message,false);}
}
function loadDemo(){
  var t=Date.now();
  var demo=[['Car','JH01AB1234','Rahul Kumar',t-75*60000,1],['Motorcycle','BR02CD5678','Priya',t-20*60000,2]];
  demo.forEach(function(d){
    if(findActive(d[1]))return;
    try{
      if(!slotOwner(d[4])){state.active.push({reg:d[1],type:d[0],owner:d[2],slot:d[4],entry:d[3]});save();}
      else{parkVehicle(d[0],d[1],d[2],d[3]);}
    }catch(e){}
  });
  showTab('dashboard');
}

/* ---- Wiring ---- */
document.querySelectorAll('nav button[data-tab]').forEach(function(b){
  b.addEventListener('click',function(){showTab(b.getAttribute('data-tab'));});
});
$('slotGrid').addEventListener('click',function(ev){
  var el=ev.target.closest('.slot');if(!el)return;
  if(el.classList.contains('busy')){showTab('exit');lookup(el.getAttribute('data-reg'));}
  else{showTab('entry');$('eReg').focus();}
});
$('parkBtn').onclick=doPark;
$('findBtn').onclick=function(){$('receiptWrap').className='empty';$('receiptWrap').textContent='The receipt will appear here after checkout.';lookup($('xReg').value);};
$('xReg').addEventListener('keydown',function(e){if(e.key==='Enter')$('findBtn').click();});
$('eOwner').addEventListener('keydown',function(e){if(e.key==='Enter')doPark();});
$('eReg').addEventListener('input',function(){this.value=cleanReg(this.value);});
$('xReg').addEventListener('input',function(){this.value=cleanReg(this.value);});
$('qBox').addEventListener('input',renderRecords);
$('demoBtn').onclick=loadDemo;
$('resetBtn').onclick=function(){
  if(confirm('Clear all parking data stored in this browser?')){state=fresh();save();currentExit=null;renderAll();}
};
window.addEventListener('storage',function(e){if(e.key===KEY){state=load();renderAll();}});
setInterval(function(){if(currentExit)renderExitInfo();},15000);

renderRates();
renderAll();
})();
</script>
</body>
</html>
)HTML_DASH";

    {
        std::ofstream out(fileName, std::ios::binary | std::ios::trunc);
        if (!out) throw std::runtime_error("Unable to write " + fileName + ".");
        out << html;
        if (!out) throw std::runtime_error("Failed while writing " + fileName + ".");
    }
    std::cout << "Dashboard generated: " << fileName << '\n';

#if defined(_WIN32)
    const std::string command = "start \"\" \"" + fileName + "\"";
#elif defined(__APPLE__)
    const std::string command = "open \"" + fileName + "\"";
#else
    const std::string command = "xdg-open \"" + fileName + "\" >/dev/null 2>&1";
#endif
    const int status = std::system(command.c_str());
    if (status != 0)
        std::cout << "Could not open the browser automatically. Open " << fileName << " manually.\n";
    else
        std::cout << "Opening ParkPilot in your default browser...\n";
}
}
int main() {
    try {
        ParkingManager manager;
        bool running=true;
        while (running) {
            printBanner();
            std::cout << "  1. Vehicle entry / allocate slot\n"
                      << "  2. Vehicle exit / generate fee receipt\n"
                      << "  3. View all parking slots\n"
                      << "  4. View currently parked vehicles\n"
                      << "  5. Occupancy report\n"
                      << "  6. Search registration\n"
                      << "  7. Completed parking / receipt history\n"
                      << "  8. Add a parking slot (admin/demo)\n"
                      << "  9. Launch ParkPilot Web Dashboard\n"
                      << "  0. Save and exit\n";
            int choice=readChoice();
            try {
                if (choice==1) {
                    std::string type=readLine("Vehicle type (Car/Motorcycle/Truck): ");
                    std::string reg=readLine("Registration number: ");
                    std::string owner=readLine("Owner name: ");
                    int slot=manager.parkVehicle(type,reg,owner);
                    std::cout << "\nSUCCESS: " << reg << " allocated slot " << slot << ".\n";
                } else if (choice==2) {
                    std::string reg=readLine("Registration number: ");
                    auto r=manager.exitVehicle(reg);
                    std::cout << "\n========== PARKING RECEIPT ==========\n"
                              << "Registration : " << r.registration << '\n'
                              << "Vehicle      : " << r.vehicleType << '\n'
                              << "Slot         : " << r.slotId << '\n'
                              << "Duration     : " << r.durationMinutes << " minute(s)\n"
                              << "Fee          : Rs. " << r.fee << '\n'
                              << "=====================================\n";
                } else if (choice==3) manager.showSlots();
                else if (choice==4) manager.showActiveVehicles();
                else if (choice==5) manager.showOccupancyReport();
                else if (choice==6) manager.searchVehicle(readLine("Registration number: "));
                else if (choice==7) manager.showHistory();
                else if (choice==8) { manager.addSlot(readLine("Category (Standard/TwoWheeler/Heavy): ")); std::cout<<"Slot added successfully.\n"; }
                else if (choice==9) generateAndLaunchDashboard();
                else if (choice==0) { running=false; std::cout<<"Data saved. Goodbye!\n"; }
                else std::cout << "Choose one of the listed options.\n";
            } catch (const std::exception& e) { std::cout << "OPERATION FAILED: " << e.what() << '\n'; }
            if (running) { std::cout << "\nPress Enter to continue..."; std::string pause; std::getline(std::cin,pause); }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n'; return 1;
    }
    return 0;
}
