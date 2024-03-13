// vite.config.ts
import { resolve } from "path";
import { defineConfig, loadEnv } from "file:///Users/admin/Documents/hypedTelemetry/telemetry/node_modules/.pnpm/vite@4.3.9_@types+node@20.10.0/node_modules/vite/dist/node/index.js";
import { viteStaticCopy } from "file:///Users/admin/Documents/hypedTelemetry/telemetry/node_modules/.pnpm/vite-plugin-static-copy@0.13.1_vite@4.3.9/node_modules/vite-plugin-static-copy/dist/index.js";
import tsconfigPaths from "file:///Users/admin/Documents/hypedTelemetry/telemetry/node_modules/.pnpm/vite-tsconfig-paths@4.2.0_typescript@5.3.3_vite@4.3.9/node_modules/vite-tsconfig-paths/dist/index.mjs";
import react from "file:///Users/admin/Documents/hypedTelemetry/telemetry/node_modules/.pnpm/@vitejs+plugin-react@3.1.0_vite@4.3.9/node_modules/@vitejs/plugin-react/dist/index.mjs";
var __vite_injected_original_dirname = "/Users/admin/Documents/hypedTelemetry/telemetry/packages/ui";
var vite_config_default = defineConfig(({ command, mode }) => {
  process.env = { ...process.env, ...loadEnv(mode, process.cwd(), "") };
  return {
    server: {
      host: process.env.HOST || "localhost",
      watch: {
        usePolling: true
      }
    },
    build: {
      rollupOptions: {
        input: {
          main: resolve(__vite_injected_original_dirname, "index.html"),
          app: resolve(__vite_injected_original_dirname, "app/index.html"),
          openmct: resolve(__vite_injected_original_dirname, "openmct/index.html")
        },
        onwarn: (warning, warn) => {
          if (warning.code === "MODULE_LEVEL_DIRECTIVE")
            return;
          warn(warning);
        }
      }
    },
    plugins: [
      viteStaticCopy({
        targets: [
          {
            src: "./node_modules/openmct/dist/*",
            dest: "openmct-lib"
          }
        ]
      }),
      tsconfigPaths(),
      react()
    ]
  };
});
export {
  vite_config_default as default
};
//# sourceMappingURL=data:application/json;base64,ewogICJ2ZXJzaW9uIjogMywKICAic291cmNlcyI6IFsidml0ZS5jb25maWcudHMiXSwKICAic291cmNlc0NvbnRlbnQiOiBbImNvbnN0IF9fdml0ZV9pbmplY3RlZF9vcmlnaW5hbF9kaXJuYW1lID0gXCIvVXNlcnMvYWRtaW4vRG9jdW1lbnRzL2h5cGVkVGVsZW1ldHJ5L3RlbGVtZXRyeS9wYWNrYWdlcy91aVwiO2NvbnN0IF9fdml0ZV9pbmplY3RlZF9vcmlnaW5hbF9maWxlbmFtZSA9IFwiL1VzZXJzL2FkbWluL0RvY3VtZW50cy9oeXBlZFRlbGVtZXRyeS90ZWxlbWV0cnkvcGFja2FnZXMvdWkvdml0ZS5jb25maWcudHNcIjtjb25zdCBfX3ZpdGVfaW5qZWN0ZWRfb3JpZ2luYWxfaW1wb3J0X21ldGFfdXJsID0gXCJmaWxlOi8vL1VzZXJzL2FkbWluL0RvY3VtZW50cy9oeXBlZFRlbGVtZXRyeS90ZWxlbWV0cnkvcGFja2FnZXMvdWkvdml0ZS5jb25maWcudHNcIjtpbXBvcnQgeyByZXNvbHZlIH0gZnJvbSAncGF0aCc7XHJcbmltcG9ydCB7IGRlZmluZUNvbmZpZywgbG9hZEVudiB9IGZyb20gJ3ZpdGUnO1xyXG5pbXBvcnQgeyB2aXRlU3RhdGljQ29weSB9IGZyb20gJ3ZpdGUtcGx1Z2luLXN0YXRpYy1jb3B5JztcclxuaW1wb3J0IHRzY29uZmlnUGF0aHMgZnJvbSAndml0ZS10c2NvbmZpZy1wYXRocyc7XHJcbmltcG9ydCByZWFjdCBmcm9tICdAdml0ZWpzL3BsdWdpbi1yZWFjdCc7XHJcblxyXG4vLyBodHRwczovL3ZpdGVqcy5kZXYvY29uZmlnL1xyXG5leHBvcnQgZGVmYXVsdCBkZWZpbmVDb25maWcoKHsgY29tbWFuZCwgbW9kZSB9KSA9PiB7XHJcbiAgcHJvY2Vzcy5lbnYgPSB7IC4uLnByb2Nlc3MuZW52LCAuLi5sb2FkRW52KG1vZGUsIHByb2Nlc3MuY3dkKCksICcnKSB9O1xyXG4gIHJldHVybiB7XHJcbiAgICBzZXJ2ZXI6IHtcclxuICAgICAgaG9zdDogcHJvY2Vzcy5lbnYuSE9TVCB8fCAnbG9jYWxob3N0JyxcclxuICAgICAgd2F0Y2g6IHtcclxuICAgICAgICB1c2VQb2xsaW5nOiB0cnVlLFxyXG4gICAgICB9LFxyXG4gICAgfSxcclxuICAgIGJ1aWxkOiB7XHJcbiAgICAgIHJvbGx1cE9wdGlvbnM6IHtcclxuICAgICAgICBpbnB1dDoge1xyXG4gICAgICAgICAgbWFpbjogcmVzb2x2ZShfX2Rpcm5hbWUsICdpbmRleC5odG1sJyksXHJcbiAgICAgICAgICBhcHA6IHJlc29sdmUoX19kaXJuYW1lLCAnYXBwL2luZGV4Lmh0bWwnKSxcclxuICAgICAgICAgIG9wZW5tY3Q6IHJlc29sdmUoX19kaXJuYW1lLCAnb3Blbm1jdC9pbmRleC5odG1sJyksXHJcbiAgICAgICAgfSxcclxuICAgICAgICBvbndhcm46ICh3YXJuaW5nLCB3YXJuKSA9PiB7XHJcbiAgICAgICAgICBpZiAod2FybmluZy5jb2RlID09PSAnTU9EVUxFX0xFVkVMX0RJUkVDVElWRScpIHJldHVybjtcclxuICAgICAgICAgIHdhcm4od2FybmluZyk7XHJcbiAgICAgICAgfSxcclxuICAgICAgfSxcclxuICAgIH0sXHJcbiAgICBwbHVnaW5zOiBbXHJcbiAgICAgIHZpdGVTdGF0aWNDb3B5KHtcclxuICAgICAgICB0YXJnZXRzOiBbXHJcbiAgICAgICAgICB7XHJcbiAgICAgICAgICAgIHNyYzogJy4vbm9kZV9tb2R1bGVzL29wZW5tY3QvZGlzdC8qJyxcclxuICAgICAgICAgICAgZGVzdDogJ29wZW5tY3QtbGliJyxcclxuICAgICAgICAgIH0sXHJcbiAgICAgICAgXSxcclxuICAgICAgfSksXHJcbiAgICAgIHRzY29uZmlnUGF0aHMoKSxcclxuICAgICAgcmVhY3QoKSxcclxuICAgIF0sXHJcbiAgfTtcclxufSk7XHJcbiJdLAogICJtYXBwaW5ncyI6ICI7QUFBbVcsU0FBUyxlQUFlO0FBQzNYLFNBQVMsY0FBYyxlQUFlO0FBQ3RDLFNBQVMsc0JBQXNCO0FBQy9CLE9BQU8sbUJBQW1CO0FBQzFCLE9BQU8sV0FBVztBQUpsQixJQUFNLG1DQUFtQztBQU96QyxJQUFPLHNCQUFRLGFBQWEsQ0FBQyxFQUFFLFNBQVMsS0FBSyxNQUFNO0FBQ2pELFVBQVEsTUFBTSxFQUFFLEdBQUcsUUFBUSxLQUFLLEdBQUcsUUFBUSxNQUFNLFFBQVEsSUFBSSxHQUFHLEVBQUUsRUFBRTtBQUNwRSxTQUFPO0FBQUEsSUFDTCxRQUFRO0FBQUEsTUFDTixNQUFNLFFBQVEsSUFBSSxRQUFRO0FBQUEsTUFDMUIsT0FBTztBQUFBLFFBQ0wsWUFBWTtBQUFBLE1BQ2Q7QUFBQSxJQUNGO0FBQUEsSUFDQSxPQUFPO0FBQUEsTUFDTCxlQUFlO0FBQUEsUUFDYixPQUFPO0FBQUEsVUFDTCxNQUFNLFFBQVEsa0NBQVcsWUFBWTtBQUFBLFVBQ3JDLEtBQUssUUFBUSxrQ0FBVyxnQkFBZ0I7QUFBQSxVQUN4QyxTQUFTLFFBQVEsa0NBQVcsb0JBQW9CO0FBQUEsUUFDbEQ7QUFBQSxRQUNBLFFBQVEsQ0FBQyxTQUFTLFNBQVM7QUFDekIsY0FBSSxRQUFRLFNBQVM7QUFBMEI7QUFDL0MsZUFBSyxPQUFPO0FBQUEsUUFDZDtBQUFBLE1BQ0Y7QUFBQSxJQUNGO0FBQUEsSUFDQSxTQUFTO0FBQUEsTUFDUCxlQUFlO0FBQUEsUUFDYixTQUFTO0FBQUEsVUFDUDtBQUFBLFlBQ0UsS0FBSztBQUFBLFlBQ0wsTUFBTTtBQUFBLFVBQ1I7QUFBQSxRQUNGO0FBQUEsTUFDRixDQUFDO0FBQUEsTUFDRCxjQUFjO0FBQUEsTUFDZCxNQUFNO0FBQUEsSUFDUjtBQUFBLEVBQ0Y7QUFDRixDQUFDOyIsCiAgIm5hbWVzIjogW10KfQo=
