import Nav from './components/nav';
import { ControlsUI } from './components/controls';

const App = () => {
  return (
    <div className="grid grid-cols-6 w-full">
      <Nav />
      <ControlsUI />
    </div>
  );
};

export default App;
