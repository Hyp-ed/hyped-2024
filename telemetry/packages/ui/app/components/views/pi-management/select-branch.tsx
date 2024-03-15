import { http } from 'openmct/core/http';
import { useQuery } from 'react-query';
import {
  Select,
  SelectContent,
  SelectGroup,
  SelectItem,
  SelectLabel,
  SelectTrigger,
  SelectValue,
} from '@/components/ui/select';
import { DEFAULT_BRANCH } from './pi-management';

const getAllBranches = async (): Promise<string[]> => {
  const res = await http.get(`info/git/all-branches`).then((res) =>
    res.json<{
      branches: string[];
    }>(),
  );
  return res.branches;
};

export const SelectBranch = ({
  setCompareBranch,
}: {
  setCompareBranch: (branch: string) => void;
}) => {
  const { data, isLoading, isRefetching, refetch } = useQuery(
    ['branches'],
    () => getAllBranches(),
  );

  const localBranches = data
    ? data.filter((branch) => !branch.startsWith('origin/'))
    : [];
  const remoteBranches = data
    ? data.filter((branch) => branch.startsWith('origin/'))
    : [];

  return (
    <Select
      defaultValue={DEFAULT_BRANCH}
      disabled={isLoading}
      onValueChange={(branch) => setCompareBranch(branch)}
    >
      <SelectTrigger className="w-[180px]">
        <SelectValue />
      </SelectTrigger>
      <SelectContent>
        <SelectGroup>
          <SelectLabel>Local Branches</SelectLabel>
          {localBranches.map((branch) => (
            <SelectItem key={branch} value={branch}>
              {branch}
            </SelectItem>
          ))}
        </SelectGroup>
        <SelectGroup>
          <SelectLabel>Remote Branches</SelectLabel>
          {remoteBranches.map((branch) => (
            <SelectItem key={branch} value={branch}>
              {branch}
            </SelectItem>
          ))}
        </SelectGroup>
      </SelectContent>
    </Select>
  );
};
