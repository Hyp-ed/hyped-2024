import { OpenMctObjectTypes } from "@hyped/telemetry-types";
import { http } from "../../core/http";

export function fetchObjectTypes() {
  return http
    .get('openmct/object-types')
    .json<OpenMctObjectTypes>()
    .then((data) => {
      return data;
    });
}