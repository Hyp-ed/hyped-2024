import { AugmentedDomainObject } from 'openmct/types/AugmentedDomainObject';
import { FAULT_MANAGEMENT_DOMAIN_TYPE } from './constants';
import { http } from 'openmct/core/http';

// David change next year :)
// TODO: Change this to be dynamic
const POD_ID = 'pod_2024';

export function HistoricalFaultsProvider() {
  return {
    supportsRequest(domainObject: AugmentedDomainObject) {
      return domainObject.type === FAULT_MANAGEMENT_DOMAIN_TYPE;
    },
    request: () => {
      const url = `openmct/faults/historical/pods/${POD_ID}`;

      return http
        .get(url)
        .json()
        .then((data: any) => data.map((fault: any) => fault.fault));
    },
  };
}
