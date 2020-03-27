/* This file is part of the OpenMV project.
 * Copyright (c) 2013-2019 Ibrahim Abdelkader <iabdalkader@openmv.io> & Kwabena W. Agyeman <kwagyeman@openmv.io>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 */

#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "libtf.h"

extern "C" {

    int libtf_get_input_data_hwc(const unsigned char *model_data,
                                 unsigned char *tensor_arena, const unsigned int tensor_arena_size,
                                 unsigned int *input_height, unsigned int *input_width, unsigned int *input_channels)
    {
        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

        const tflite::Model *model = tflite::GetModel(model_data);

        if (model->version() != TFLITE_SCHEMA_VERSION) {
            error_reporter->Report("Model provided is schema version is not equal to supported version!");
            return 1;
        }

        tflite::ops::micro::AllOpsResolver resolver;
        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, tensor_arena_size, error_reporter);

        if (interpreter.AllocateTensors() != kTfLiteOk) {
            error_reporter->Report("AllocateTensors() failed!");
            return 1;
        }

        TfLiteTensor *model_input = interpreter.input(0);

#if 0
        if (model_input->type != kTfLiteUInt8) {
            error_reporter->Report("Input model data type should be 8-bit quantized!");
            return 1;
        }
#endif

        if (model_input->dims->size == 2) {

            *input_height = model_input->dims->data[0];
            *input_width = model_input->dims->data[1];
            *input_channels = 1;

            return 0;

        } else if (model_input->dims->size == 3) {

            if ((model_input->dims->data[2] != 1) && (model_input->dims->data[2] != 3)) {
                error_reporter->Report("Input dimension [c] should be 1 or 3!");
                return 1;
            }

            *input_height = model_input->dims->data[0];
            *input_width = model_input->dims->data[1];
            *input_channels = model_input->dims->data[2];

            return 0;

        } else if (model_input->dims->size == 4) {

            if (model_input->dims->data[0] != 1) {
                error_reporter->Report("Input dimension [n] should be 1!");
                return 1;
            }

            if ((model_input->dims->data[3] != 1) && (model_input->dims->data[3] != 3)) {
                error_reporter->Report("Input dimension [c] should be 1 or 3!");
                return 1;
            }

            *input_height = model_input->dims->data[1];
            *input_width = model_input->dims->data[2];
            *input_channels = model_input->dims->data[3];

            return 0;

        } else {
            error_reporter->Report("Input dimensions should be [h][w](c=1), [h][w][c==1||c==3], or [n==1][h][w][c==1||c==3]!");
            return 1;
        }
    }

    int libtf_get_output_data_hwc(const unsigned char *model_data,
                                  unsigned char *tensor_arena, const unsigned int tensor_arena_size,
                                  unsigned int *output_height, unsigned int *output_width, unsigned int *output_channels)
    {
        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

        const tflite::Model *model = tflite::GetModel(model_data);

        if (model->version() != TFLITE_SCHEMA_VERSION) {
            error_reporter->Report("Model provided is schema version is not equal to supported version!");
            return 1;
        }

        tflite::ops::micro::AllOpsResolver resolver;
        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, tensor_arena_size, error_reporter);

        if (interpreter.AllocateTensors() != kTfLiteOk) {
            error_reporter->Report("AllocateTensors() failed!");
            return 1;
        }

        TfLiteTensor *model_output = interpreter.output(0);

#if 0
        if (model_output->type != kTfLiteUInt8) {
            error_reporter->Report("Output model data type should be 8-bit quantized!");
            return 1;
        }
#endif

        if (model_output->dims->size == 1) {

            *output_height = 1;
            *output_width = 1;
            *output_channels = model_output->dims->data[0];

            return 0;

        } else if (model_output->dims->size == 2) {

            if (model_output->dims->data[0] != 1) {
                error_reporter->Report("Output dimension [n] should be 1!");
                return 1;
            }

            *output_height = 1;
            *output_width = 1;
            *output_channels = model_output->dims->data[1];

            return 0;

        } else if (model_output->dims->size == 3) {

            *output_height = model_output->dims->data[0];
            *output_width = model_output->dims->data[1];
            *output_channels = model_output->dims->data[2];

            return 0;

        } else if (model_output->dims->size == 4) {

            if (model_output->dims->data[0] != 1) {
                error_reporter->Report("Output dimension [n] should be 1!");
                return 1;
            }

            *output_height = model_output->dims->data[1];
            *output_width = model_output->dims->data[2];
            *output_channels = model_output->dims->data[3];

            return 0;

        } else {
            error_reporter->Report("Output dimensions should be [c], [n==1][c], [h][w][c], or [n==1][h][w][c]!");
            return 1;
        }
    }

    int libtf_invoke(const unsigned char *model_data,
                     unsigned char *tensor_arena, const unsigned int tensor_arena_size,
                     libtf_input_data_callback_t input_callback, void *input_callback_data,
                     libtf_output_data_callback_t output_callback, void *output_callback_data)
    {
        tflite::MicroErrorReporter micro_error_reporter;
        tflite::ErrorReporter *error_reporter = &micro_error_reporter;

        const tflite::Model *model = tflite::GetModel(model_data);

        if (model->version() != TFLITE_SCHEMA_VERSION) {
            error_reporter->Report("Model provided is schema version is not equal to supported version!");
            return 1;
        }

        tflite::ops::micro::AllOpsResolver resolver;
        tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, tensor_arena_size, error_reporter);

        if (interpreter.AllocateTensors() != kTfLiteOk) {
            error_reporter->Report("AllocateTensors() failed!");
            return 1;
        }

        TfLiteTensor *model_input = interpreter.input(0);

#if 0
        if (model_input->type != kTfLiteUInt8) {
            error_reporter->Report("Input model data type should be 8-bit quantized!");
            return 1;
        }
#endif

        if (model_input->dims->size == 2) {

            input_callback(input_callback_data,
                           model_input->data.f,
                           model_input->dims->data[0],
                           model_input->dims->data[1],
                           1);

        } else if (model_input->dims->size == 3) {

            if ((model_input->dims->data[2] != 1) && (model_input->dims->data[2] != 3)) {
                error_reporter->Report("Input dimension [c] should be 1 or 3!");
                return 1;
            }

            input_callback(input_callback_data,
                           model_input->data.f,
                           model_input->dims->data[0],
                           model_input->dims->data[1],
                           model_input->dims->data[2]);

        } else if (model_input->dims->size == 4) {

            if (model_input->dims->data[0] != 1) {
                error_reporter->Report("Input dimension [n] should be 1!");
                return 1;
            }

            if ((model_input->dims->data[3] != 1) && (model_input->dims->data[3] != 3)) {
                error_reporter->Report("Input dimension [c] should be 1 or 3!");
                return 1;
            }

            input_callback(input_callback_data,
                           model_input->data.f,
                           model_input->dims->data[1],
                           model_input->dims->data[2],
                           model_input->dims->data[3]);

        } else {
            error_reporter->Report("Input dimensions should be [h][w](c=1), [h][w][c==1||c==3], or [n==1][h][w][c==1||c==3]!");
            return 1;
        }

        if (interpreter.Invoke() != kTfLiteOk) {
            error_reporter->Report("Invoke() failed!");
            return 1;
        }

        TfLiteTensor *model_output = interpreter.output(0);

#if 0
        if (model_output->type != kTfLiteUInt8) {
            error_reporter->Report("Output model data type should be 8-bit quantized!");
            return 1;
        }
#endif

        if (model_output->dims->size == 1) {

            output_callback(output_callback_data,
                            model_output->data.f,
                            1,
                            1,
                            model_output->dims->data[0]);

        } else if (model_output->dims->size == 2) {

            if (model_output->dims->data[0] != 1) {
                error_reporter->Report("Output dimension [n] should be 1!");
                return 1;
            }

            output_callback(output_callback_data,
                            model_output->data.f,
                            1,
                            1,
                            model_output->dims->data[1]);

        } else if (model_output->dims->size == 3) {

            output_callback(output_callback_data,
                            model_output->data.f,
                            model_output->dims->data[0],
                            model_output->dims->data[1],
                            model_output->dims->data[2]);

        } else if (model_output->dims->size == 4) {

            if (model_output->dims->data[0] != 1) {
                error_reporter->Report("Output dimension [n] should be 1!");
                return 1;
            }

            output_callback(output_callback_data,
                            model_output->data.f,
                            model_output->dims->data[1],
                            model_output->dims->data[2],
                            model_output->dims->data[3]);

        } else {
            error_reporter->Report("Output dimensions should be [c], [n==1][c], [h][w][c], or [n==1][h][w][c]!");
            return 1;
        }

        return 0;
    }
}
