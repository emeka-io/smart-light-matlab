% -- MATLAB Command Window Code To Plot The Live Graph --
port = "COM4"; % To use this source code ensure this matches your Arduino port
baud = 9600;

% Clear existing ports to avoid "Port Busy" errors
if ~isempty(serialportlist("available"))
    clear s;
end

s = serialport(port, baud);
configureTerminator(s, "LF");
flush(s);

% -- SETUP PLOT --
fig = figure('Name', 'LDR & LED Hardware Sync', 'Color', [0.15 0.15 0.15]);
ax = gca;
h = animatedline('LineWidth', 3);

% This styles the dark-mode graph
ax.Color = [0.1 0.1 0.1];
ax.XColor = 'w';
ax.YColor = 'w';
ax.YGrid = 'on';
ax.XGrid = 'on';
ax.GridColor = [0.3 0.3 0.3];

xlabel("Readings");
ylabel("LDR Intensity");
title("Real-Time Plot of Arduino Data", 'Color', 'w');

count = 0;
windowSize = 100;

% -- MAIN LOOP --
while isvalid(fig)
    if s.NumBytesAvailable > 0
        data = readline(s);
        
        % Parse the "Value,Status" string
        parts = strsplit(data, ',');
        
        if length(parts) == 2
            valLDR = str2double(parts(1));
            stateLED = str2double(parts(2));

            if ~isnan(valLDR)
                count = count + 1;
                addpoints(h, count, valLDR);

                % COLOR CHANGE LOGIC
                % Green when Arduino confirms LED is ON (1), Red when OFF (0)
                if stateLED == 1
                    h.Color = [0 1 0]; % Bright Green
                else
                    h.Color = [1 0 0]; % Bright Red
                end

                % Auto-scroll logic
                if count > windowSize
                    xlim([count - windowSize, count]);
                else
                    xlim([0, windowSize]);
                end
                
                drawnow limitrate;
            end
        end
    end
end
