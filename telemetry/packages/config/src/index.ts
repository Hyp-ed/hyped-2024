import { parse } from 'smol-toml';
import { readFileSync } from 'fs';

const filename = 'pod_ness.toml';
const data = parse(readFileSync(`../config/${filename}`, 'utf8'));
