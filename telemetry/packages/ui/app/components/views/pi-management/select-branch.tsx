import { http } from 'openmct/core/http';
import { useQuery } from 'react-query';
import {
  Command,
  CommandEmpty,
  CommandGroup,
  CommandInput,
  CommandItem,
  CommandList,
  CommandSeparator,
} from '@/components/ui/command';
import {
  Popover,
  PopoverContent,
  PopoverTrigger,
} from '@/components/ui/popover';
import { Button } from '@/components/ui/button';
import { useState } from 'react';
import { Check, ChevronsUpDown } from 'lucide-react';
import { cn } from '@/lib/utils';

const getAllBranches = async (): Promise<string[]> => {
  const res = await http.get(`info/git/all-branches`).then((res) =>
    res.json<{
      branches: string[];
    }>(),
  );
  return res.branches;
};

const DEFAULT_BRANCH = 'master';

export const SelectBranch = () => {
  const [open, setOpen] = useState(false);
  const [value, setValue] = useState(DEFAULT_BRANCH);

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
    <Popover open={open} onOpenChange={setOpen}>
      <PopoverTrigger asChild>
        <Button
          variant="outline"
          role="combobox"
          aria-expanded={open}
          className="w-[200px] justify-between"
        >
          {value}
          <ChevronsUpDown className="ml-2 h-4 w-4 shrink-0 opacity-50" />
        </Button>
      </PopoverTrigger>
      <PopoverContent className="w-[200px] p-0">
        <Command>
          <CommandInput placeholder="Search branches..." />
          <CommandList>
            <CommandEmpty>No branch found.</CommandEmpty>
            <CommandGroup>
              {localBranches.map((branch) => (
                <CommandItem
                  key={branch}
                  value={branch}
                  onSelect={() => {
                    setValue(branch);
                    setOpen(false);
                  }}
                >
                  <Check
                    className={cn(
                      'mr-2 h-4 w-4',
                      value === branch ? 'opacity-100' : 'opacity-0',
                    )}
                  />
                  {branch}
                </CommandItem>
              ))}
              <CommandSeparator />
              {remoteBranches.map((branch) => (
                <CommandItem
                  key={branch}
                  value={branch}
                  onSelect={() => {
                    setValue(branch);
                    setOpen(false);
                  }}
                >
                  <Check
                    className={cn(
                      'mr-2 h-4 w-4',
                      value === branch ? 'opacity-100' : 'opacity-0',
                    )}
                  />
                  {branch}
                </CommandItem>
              ))}
            </CommandGroup>
          </CommandList>
        </Command>
      </PopoverContent>
    </Popover>
  );
};
