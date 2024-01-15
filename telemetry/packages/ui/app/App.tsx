import { useState } from 'react';
import { VIEWS } from './views';
import { Sidebar } from './components/sidebar/sidebar';
import { cn } from './lib/utils';
import {
  ResizableHandle,
  ResizablePanel,
  ResizablePanelGroup,
} from '@/components/ui/resizable';

const App = () => {
  const [selectedComponent, setSelectedComponent] = useState(0);

  return (
    <ResizablePanelGroup
      direction="horizontal"
      className="w-full bg-hyped-background"
    >
      <ResizablePanel defaultSize={85} minSize={50}>
        {VIEWS.map((component) => (
          <div
            className={cn(
              'h-[100vh] w-full col-span-7 p-1',
              !(VIEWS.indexOf(component) === selectedComponent) && 'hidden',
            )}
          >
            {component.component}
          </div>
        ))}
      </ResizablePanel>
      <ResizableHandle withHandle className="bg-openmct-light-gray" />
      <ResizablePanel defaultSize={15} minSize={10}>
        <Sidebar
          selectedComponent={selectedComponent}
          setSelectedComponent={setSelectedComponent}
        />
      </ResizablePanel>
    </ResizablePanelGroup>
  );
};

export default App;
