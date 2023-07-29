import { Limits } from "../pods/pods.types";

export type OpenMctMeasurement = {
  name: string;
  key: string;
  type: string;
  values: {
    key: string;
    name: string;
    unit?: string;
    format: string;
    min?: number;
    max?: number;
    limits?: Limits;
    enumerations?: {
      value: number;
      string: string;
    }[];
    hints?: {
      range?: number;
      domain?: number;
    };
    source?: string;
    units?: {
      domain: string;
    };
  }[];
};

export type OpenMctPod = {
  id: string;
  name: string;
  measurements: OpenMctMeasurement[];
};

export type OpenMctDictionary = Record<string, OpenMctPod>;