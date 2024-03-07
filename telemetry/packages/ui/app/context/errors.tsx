import { createContext, useContext, useState } from 'react';

type ErrorMessage = {
  title: string;
  message: string;
  timestamp: number;
  acknowledge: () => void;
};

type ErrorContextType = {
  errors: ErrorMessage[];
  raiseError: (title: string, message: string) => void;
};

const ErrorContext = createContext<ErrorContextType | null>(null);

interface ErrorProviderProps {
  children: React.ReactNode;
}

/**
 * Error Context Provider.
 * Provides a context for error messages, including functions to acknowledge them, and a function to raise new errors.
 */
export const ErrorProvider = ({ children }: ErrorProviderProps) => {
  const [errors, setErrors] = useState<ErrorMessage[]>([]);

  const raiseError = (title: string, message: string) => {
    const error: ErrorMessage = {
      title,
      message,
      timestamp: Date.now(),
      acknowledge: () => {
        setErrors((errors) => errors.filter((e) => title !== e.title));
      },
    };

    // TODO: log the error

    // Add the error to the list of errors, sorted by timestamp
    setErrors((errors) =>
      [...errors, error]
        // remove duplicates
        .filter((e, i, a) => a.findIndex((ee) => ee.title === e.title) === i)
        // sort by most recent
        .sort((a, b) => b.timestamp - a.timestamp),
    );
  };

  return (
    <ErrorContext.Provider value={{ errors, raiseError }}>
      {children}
    </ErrorContext.Provider>
  );
};

export const useErrors = () => {
  const context = useContext(ErrorContext);
  if (!context) {
    throw new Error('useErrors must be used within ErrorContextProvider');
  }
  return context;
};
