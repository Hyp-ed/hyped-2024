import { utils } from "../../index";

// getting bugs from logistic and potentially random noise methods
// file to test with simple data

// Physical, fixed constraints
const MAX_ACCL = 5;
const VEL_ST_STATE = 0.95 * 50; // desired velocity leaving room for error within limiting bounds
const RMS_NOISE = 16.25 * 10**(-3) // for random noise
const TRACK_LENGTH = 100; // m

// ICs
const INIT_ACCL = 0;
const INIT_VEL = 0.3; // 
const INIT_DISP = 0;

// Time step
const DT = 500 // ms

// Logistic params
const K = 0.4; // growth rate factor
const T_INF = 12.5; // time of inflection (s)

// Setup dynamic motion variables
let accl = INIT_ACCL;
let vel = INIT_VEL;
let disp = INIT_DISP;

let t = 0;
while (disp <= TRACK_LENGTH) { 
  const velEstimate = (utils.logistic((t / 1000), VEL_ST_STATE, K, T_INF));
    
  console.log(`time: ${t / 1000} s\t -> \tlogst. velocity: ${round2DP(vel)} m/s\t`);
  
  accl = (velEstimate - vel) / (DT / 1000);
  // ensure acceleration is within limits (maybe remove this so we can see if it escapes limits and we can prevent it?)
  accl = accl <= MAX_ACCL ? accl : MAX_ACCL;
  // add random reading noise
  accl += utils.gaussianRandom(RMS_NOISE);
  vel += accl * (DT / 1000);
  disp += vel * (DT / 1000);

  console.log(`|\taccl: ${round2DP(accl)} m/s² \tvel: ${round2DP(vel)} m/s \tdisp: ${round2DP(disp)} m\t\n`);
  t += DT;
};

function round2DP(num: number): number {
  return parseFloat(num.toFixed(2));
}

console.log('Simulation complete');
console.log(`Velocity reached: ${vel} m/s`);
console.log(`Time of run completion was ${t} seconds`);