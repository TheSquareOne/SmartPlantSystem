        // Function to update slider value based on text field value
        function updateSliderValueFromTextField(textFieldId, sliderId) {
            var textField = document.getElementById(textFieldId);
            var slider = document.getElementById(sliderId);

            slider.value = textField.value;
        }

        // Function to update text field value based on slider value
        function updateTextFieldValueFromSlider(sliderId, textFieldId) {
            var slider = document.getElementById(sliderId);
            var textField = document.getElementById(textFieldId);

            textField.value = slider.value;
        }

        // Add event listeners to each slider and text field
        document.getElementById('watering_threshold_slider').addEventListener('input', function() {
            updateTextFieldValueFromSlider('watering_threshold_slider', 'watering_threshold');
        });

        document.getElementById('watering_threshold').addEventListener('input', function() {
            updateSliderValueFromTextField('watering_threshold', 'watering_threshold_slider');
        });

        document.getElementById('watering_duration_slider').addEventListener('input', function() {
            updateTextFieldValueFromSlider('watering_duration_slider', 'watering_duration');
        });

        document.getElementById('watering_duration').addEventListener('input', function() {
            updateSliderValueFromTextField('watering_duration', 'watering_duration_slider');
        });

        document.getElementById('watering_repeat_slider').addEventListener('input', function() {
            updateTextFieldValueFromSlider('watering_repeat_slider', 'watering_repeat');
        });

        document.getElementById('watering_repeat').addEventListener('input', function() {
            updateSliderValueFromTextField('watering_repeat', 'watering_repeat_slider');
        });

        document.getElementById('watering_soak_time_slider').addEventListener('input', function() {
            updateTextFieldValueFromSlider('watering_soak_time_slider', 'watering_soak_time');
        });

        document.getElementById('watering_soak_time').addEventListener('input', function() {
            updateSliderValueFromTextField('watering_soak_time', 'watering_soak_time_slider');
        });

        document.getElementById('watering_interval_slider').addEventListener('input', function() {
            updateTextFieldValueFromSlider('watering_interval_slider', 'watering_interval');
        });

        document.getElementById('watering_interval').addEventListener('input', function() {
            updateSliderValueFromTextField('watering_interval', 'watering_interval_slider');
        });