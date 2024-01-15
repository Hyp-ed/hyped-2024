import { pods } from '@hyped/telemetry-constants';
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
  const { currentPod: podId, setPod } = useCurrentPod();

  const podOptions = Object.keys(pods).map((podId) => getDisplayText(podId));

  return (
    <div className="space-y-2">
      <Label htmlFor="pod-select">Select Pod:</Label>
      <Select
        value={getDisplayText(podId)}
        onValueChange={(v) => setPod(v.split(' - ')[0])}
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

const getDisplayText = (podId: string) => `${pods[podId].name} (${podId})`;
