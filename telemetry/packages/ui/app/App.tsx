import { useState } from 'react';
import { VIEWS } from './views';
import { ControlsUI } from './components/controls';
import { cn } from './lib/utils';

const App = () => {
  const [selectedComponent, setSelectedComponent] = useState(0);

  return (
    <div className="grid grid-cols-8 bg-hyped-background">
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
      <ControlsUI
        selectedComponent={selectedComponent}
        setSelectedComponent={setSelectedComponent}
      />
    </div>
  );
};

export default App;
