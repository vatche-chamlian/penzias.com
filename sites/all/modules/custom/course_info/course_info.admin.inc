<?php

function course_info_config_form($form, &$form_state){
    
    $form['course_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Course ID'),
    '#default_value' => variable_get('course_id', ''),
    '#description' => t(''),
    );
    
    $form['course_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Course Prefix'),
    '#default_value' => variable_get('course_prefix', ''),
    '#description' => t(''),
    );
    
    $form['course_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Course Name'),
    '#default_value' => variable_get('course_name', ''),
    '#description' => t(''),
    );
    
    $form['course_slogan'] = array(
    '#type' => 'textfield',
    '#title' => t('Course Slogan'),
    '#default_value' => variable_get('course_slogan', ''),
    '#description' => t(''),
    );
    
    $form['course_venue_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Course Venue'),
    '#default_value' => variable_get('course_venue_name', ''),
    '#description' => t(''),
    );
    
    $form['course_city'] = array(
    '#type' => 'textfield',
    '#title' => t('Course City'),
    '#default_value' => variable_get('course_city', ''),
    '#description' => t(''),
    );
    
    $form['course_state'] = array(
    '#type' => 'textfield',
    '#title' => t('Course State'),
    '#default_value' => variable_get('course_state', ''),
    '#description' => t(''),
    );
       
    $start_date = variable_get('course_start_date', array(
        'month' => format_date(time(), 'custom', 'n'),
        'day' => format_date(time(), 'custom', 'j'),
        'year' => format_date(time(), 'custom', 'Y'),
        ));
    
    $form['course_start_date'] = array(
    '#type' => 'date',
    '#title' => t('Course Start Date'),
    '#default_value' => array(
        'month' => $start_date['month'],
        'day' => $start_date['day'],
        'year' => $start_date['year'],
    ),
    '#description' => t(''),
    );
    
    $end_date = variable_get('course_end_date', array(
        'month' => format_date(time(), 'custom', 'n'),
        'day' => format_date(time(), 'custom', 'j'),
        'year' => format_date(time(), 'custom', 'Y'),
        ));
    
    $form['course_end_date'] = array(
    '#type' => 'date',
    '#title' => t('Course End Date'),
    '#default_value' => array(
        'month' => $end_date['month'],
        'day' => $end_date['day'],
        'year' => $end_date['year'],
    ),
    '#description' => t(''),
    );
    
    //Sold Out
    
    $form['course_sold_out'] = array(
        '#type' => 'checkbox',
        '#title' => t('Course Sold Out'),
        '#default_value' => variable_get('course_sold_out'),
    );
    
    //Early Bird Dates
    $earlybird_start_date = variable_get('earlybird_start_date', array(
        'month' => format_date(time(), 'custom', 'n'),
        'day' => format_date(time(), 'custom', 'j'),
        'year' => format_date(time(), 'custom', 'Y'),
        ));
    
    $form['earlybird_start_date'] = array(
    '#type' => 'date',
    '#title' => t('Early Bird Start Date'),
    '#default_value' => array(
        'month' => $earlybird_start_date['month'],
        'day' => $earlybird_start_date['day'],
        'year' => $earlybird_start_date['year'],
    ),
    '#description' => t(''),
    );
    
    $earlybird_end_date = variable_get('earlybird_end_date', array(
        'month' => format_date(time(), 'custom', 'n'),
        'day' => format_date(time(), 'custom', 'j'),
        'year' => format_date(time(), 'custom', 'Y'),
        ));
    
    $form['earlybird_end_date'] = array(
    '#type' => 'date',
    '#title' => t('Early Bird End Date'),
    '#default_value' => array(
        'month' => $earlybird_end_date['month'],
        'day' => $earlybird_end_date['day'],
        'year' => $earlybird_end_date['year'],
    ),
    '#description' => t(''),
    );
    
     // Use the #managed_file FAPI element to upload a file.
    $form['brochure_fid'] = array(
      '#title' => t('File'),
      '#type' => 'managed_file',
      '#description' => t('The uploaded file will be processed.'),
      '#default_value' => variable_get('brochure_fid', ''),
      '#upload_validators' => array(
      'file_validate_extensions' => array('pdf'),
      ),
    '#upload_location' => 'public://course-info/',
    );
    
    $form['#submit'][] = 'handle_brochure';
    
    return system_settings_form($form);
}

function handle_brochure(&$form, &$form_state) { 
    
    //If a file existed before the form was submitted and it is not the same one, delete the old one.
    if((!empty($form_state['complete form']['brochure_fid']['#default_value']) && $form_state['complete form']['brochure_fid']['#default_value'] != $form_state['values']['brochure_fid'])){
        
        //Load previous file
        $file = file_load($form_state['complete form']['brochure_fid']['#default_value']);
        
        //Remove from usage table
        file_usage_delete($file,'course_info');
        
        //Blast that sucker out of orbit.
        file_delete($file);
    
    }
    
    //If there's a new value, save the file
    if(!empty($form_state['values']['brochure_fid'])){
        
        // Load the file via file.fid.
        $file = file_load($form_state['values']['brochure_fid']);

        // We are done with the file, remove it from storage.
        unset($form_state['storage']['file']);

        // Change status to permanent.
        $file->status = FILE_STATUS_PERMANENT; 
        
        // Save the file
        $uploaded = file_save($file);
        
        //If it all worked out then display a happy message and set the file usage
        if ($uploaded == TRUE) {
          // Record that the module (in this example, user module) is using the file. 
          file_usage_add($file, 'course_info', 'brochure', 1);
          drupal_set_message(t('Course brochure successfully uploaded.'));
        }
        else {
          drupal_set_message(t('The course brochure could not be uploaded. Please contact the site administrator.'), 'error');
        }
    }
}