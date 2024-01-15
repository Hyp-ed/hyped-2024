import { POD_IDS, PodId, pods } from '@hyped/telemetry-constants';
import { Label } from './ui/label';
import {
  Select,
  SelectContent,
  SelectItem,
  SelectTrigger,
  SelectValue,
} from './ui/select';
import { useCurrentPod } from '@/context/pods';

export const PodSelector = () => {
  const { currentPod: podId, setCurrentPod } = useCurrentPod();

  const podOptions = POD_IDS.map((podId) => getDisplayText(podId));

  return (
    <div className="space-y-2">
      <Label htmlFor="pod-select">Select Pod:</Label>
      <Select
        value={getDisplayText(podId)}
        onValueChange={(v) => setCurrentPod(getPodIdFromDisplayText(v))}
      >
        <SelectTrigger id="pod-select" className="w-[180px]">
          <SelectValue />
        </SelectTrigger>
        <SelectContent>
          {podOptions.map((podOption) => (
            <SelectItem key={podOption} value={podOption}>
              {podOption}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    </div>
  );
};

const getDisplayText = (podId: PodId) => `${pods[podId].name} (${podId})`;
const getPodIdFromDisplayText = (displayText: string) => {
  const podId = displayText.split('(')[1].split(')')[0];
  return podId as PodId;
};
