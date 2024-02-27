"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var __1 = require("../");
// getting bugs from logistic and potentially random noise methods
// file to test with simple data
// Physical, fixed constraints
var MAX_ACCL = 5;
var VEL_ST_STATE = 0.95 * 50; // desired velocity leaving room for error within limiting bounds
var RMS_NOISE = 16.25 * Math.pow(10, -3); // for random noise
var TRACK_LENGTH = 100; // m
// ICs
var INIT_ACCL = 0;
var INIT_VEL = 0.3; //
var INIT_DISP = 0;
// Time step
var DT = 500; // ms
// Logistic params
var K = 0.4; // growth rate factor
var T_INF = 12.5; // time of inflection (s)
// Setup dynamic motion variables
var accl = INIT_ACCL;
var vel = INIT_VEL;
var disp = INIT_DISP;
var t = 0;
while (disp <= TRACK_LENGTH) {
    var velEstimate = __1.utils.logistic(t / 1000, VEL_ST_STATE, K, T_INF);
    console.log("time: ".concat(t / 1000, " s\t -> \tlogst. velocity: ").concat(round2DP(vel), " m/s\t"));
    accl = (velEstimate - vel) / (DT / 1000);
    // ensure acceleration is within limits (maybe remove this so we can see if it escapes limits and we can prevent it?)
    accl = accl <= MAX_ACCL ? accl : MAX_ACCL;
    // add random reading noise
    accl += __1.utils.gaussianRandom(RMS_NOISE);
    vel += accl * (DT / 1000);
    disp += vel * (DT / 1000);
    console.log("|\taccl: ".concat(round2DP(accl), " m/s\u00B2 \tvel: ").concat(round2DP(vel), " m/s \tdisp: ").concat(round2DP(disp), " m\t\n"));
    t += DT;
}
function round2DP(num) {
    return parseFloat(num.toFixed(2));
}
console.log('Simulation complete');
console.log("Velocity reached: ".concat(vel, " m/s"));
console.log("Time of run completion was ".concat(t, " seconds"));
