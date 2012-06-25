<?php

/**
 * Implements hook_rules_action_info() on behalf of the system module.
 */
function purr_messages_rules_action_info() {
  return array(
    'purr_message' => array(
      'label' => t('Show a purr style message on the site'),
      'group' => t('System'),
      'parameter' => array(
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
          'sanitize' => TRUE,
        ),
        'repeat' => array(
          'type' => 'boolean',
          'label' => t('Repeat message'),
          'description' => t("If disabled and the message has been already shown, 
            then the message won't be repeated."),
          'default value' => TRUE,
          'optional' => TRUE,
          'restriction' => 'input',
        ),
      ),
      'base' => 'purr_messages_drupal_message',
      'access callback' => 'rules_system_integration_access',
    ),
  );
}


/**
 * Action: Show a purr style message.
 */
function purr_messages_drupal_message($message, $repeat) {
  drupal_set_message(filter_xss_admin($message), 'purr', $repeat);
}